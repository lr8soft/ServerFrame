//
// Created by lrsof on 2023/4/17.
//

#include <lua.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <cmath>
#include "LuaResolver.h"
#include "../Core/Request.hpp"
#include "../Core/Reply.h"
#include "../Utils/LuaUtil.h"
#include "../Utils/LogUtil.h"
#include "../Utils/UrlUtils.h"

LuaResolver::LuaResolver() {
    pState = LuaUtil::getNewState();
#ifdef _DEBUG
    const char* path = "../scripts/manage.lua";
#else
    const char* path = "scripts/manage.lua";
#endif
    luaL_openlibs(pState);
    luaL_dofile(pState, path);
    lua_getglobal(pState, "url");
    lua_pushnil(pState);
    // lua_next弹出url表里的名称与方法
    while (lua_next(pState, -2) != 0) {
        const char *key = lua_tostring(pState, -2);
        if (lua_isfunction(pState, -1)) {
            urlMethodMap.insert(std::make_pair(key, 0));
            //std::cout << " key:" << key << std::endl;
        }
        lua_pop(pState, 1);
    }
}

LuaResolver::~LuaResolver() {
    LuaUtil::luaEnvironmentRelease(pState);
    pState = nullptr;
}

bool LuaResolver::handleRequest(const Request &req, Reply &rep) {
    std::string reqPath;
    // 解析失败
    if (!UrlUtils::urlDecode(req.uri, reqPath)) {
        rep = Reply::stockReply(Reply::bad_request);
        return false;
    }

    // 去掉斜杠
    reqPath = reqPath.substr(1);
    // 从urlMethodMap中找到对应的方法
    auto it = urlMethodMap.find(reqPath);
    if (it == urlMethodMap.end()) {
        rep = Reply::stockReply(Reply::not_found);
        return false;
    }

    lua_getglobal(pState, "url");
    lua_pushstring(pState, it->first.c_str());
    lua_gettable(pState, -2);

    if (!lua_isfunction(pState, -1)) {
        LogUtil::printError("Fail to get lua function " + it->first);
        rep = Reply::stockReply(Reply::internal_server_error);
        return false;
    }

    lua_newtable(pState);
    int tableIndex = lua_gettop(pState);
    // 遍历req.contentMap并push到Lua表中
    for (auto const &[key, val]: req.bodyMap) {
        lua_pushstring(pState, key.c_str());
        lua_pushstring(pState, val.c_str());
        lua_settable(pState, tableIndex);
    }

    // 调用Lua方法
    if (lua_pcall(pState, 1, 1, 0) != LUA_OK) {
        LogUtil::printError("Fail to call lua function " + it->first);
        rep = Reply::stockReply(Reply::internal_server_error);
        return false;
    }
    // 返回的都是table
    if (!lua_istable(pState, -1)) {
        LogUtil::printError("Fail to get lua table " + it->first);
        rep = Reply::stockReply(Reply::internal_server_error);
        return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    parseLuaTable(pState,  writer, lua_gettop(pState));
    // 弹出返回的table
    lua_pop(pState, 1);

    // 设置json并返回
    Reply::setReply(rep, buffer.GetString(), "json");
    return true;
}

void LuaResolver::parseLuaTable(lua_State *pState, rapidjson::Writer<rapidjson::StringBuffer> &writer, int index) {
    lua_pushnil(pState); // 第一个键
    writer.StartObject();
    while (lua_next(pState, index) != 0) {
        // 使用lua_tostring函数获取键和值
        writer.Key(lua_tostring(pState, -2));
        if (lua_istable(pState, -1)) {
            // 如果值是一个表，递归调用自己
            parseLuaTable(pState, writer, lua_gettop(pState));
        } else {
            switch (lua_type(pState, -1)) {
                case LUA_TSTRING:
                    writer.String(lua_tostring(pState, -1));
                    break;
                case LUA_TBOOLEAN:
                    writer.Bool(lua_toboolean(pState, -1));
                    break;
                case LUA_TNUMBER: {
                    double value = lua_tonumber(pState, -1);
                    if(ceil(value) == floor(value))
                        writer.Int((int)value);
                    else
                        writer.Double(lua_tonumber(pState, -1));
                    break;
                }
                default:
                    writer.Null();
                    break;
            }
        }
        // 弹出值，保留键用于下一次迭代
        lua_pop(pState, 1);
    }
    writer.EndObject();
}