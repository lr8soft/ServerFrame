//
// Created by lrsof on 2023/4/17.
//

#include <lua.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
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
    writer.StartObject();
    // 回到栈顶
    int returnTableIndex = lua_gettop(pState);
    lua_pushnil(pState);
    while (lua_next(pState, returnTableIndex) != 0) {
        const char *key = lua_tostring(pState, -2);
        std::string value = parserAnyValue(pState, -1);
        writer.Key(key);
        writer.String(value.c_str());
        // 弹出key
        lua_pop(pState, 1);
    }
    // 弹出返回的table
    lua_pop(pState, 1);

    writer.EndObject();
    // 设置json并返回
    Reply::setReply(rep, buffer.GetString(), "json");
    return true;
}

std::string LuaResolver::parserAnyValue(lua_State* state, int index) {
    if (lua_isnumber(state, index)) {
        double value = lua_tonumber(state, index);
        return std::to_string(value);
    } else if (lua_isstring(state, index)) {
        const char* value = lua_tostring(state, index);
        return value;
    } else if (lua_isboolean(state, index)) {
        bool value = lua_toboolean(state, index);
        return std::to_string(value);
    } else if (lua_istable(state, index)) {
        // 处理嵌套的table
    }

    return "";
}




