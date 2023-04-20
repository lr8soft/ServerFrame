//
// Created by lrsof on 2023/4/17.
//

#include <lua.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <cmath>
#include <string>
#include "LuaResolver.h"
#include "../Core/Request.hpp"
#include "../Core/Reply.h"
#include "../Utils/LuaUtil.h"
#include "../Utils/LogUtil.h"
#include "../Utils/UrlUtils.h"

LuaResolver::LuaResolver() {
    pState = LuaUtil::getNewState();
    luaL_openlibs(pState);
#ifdef _DEBUG
    const char* packageStr = "package.path = package.path .. ';../scripts/?.lua'";
    const char* path = "../scripts/manage.lua";
#else
    const char* packageStr = "package.path = package.path .. ';./scripts/?.lua'";
    const char* path = "scripts/manage.lua";
#endif

    luaL_dostring(pState, packageStr);
    luaL_openlibs(pState);
    if(luaL_dofile(pState, path) == LUA_OK) {
        lua_getglobal(pState, "url");

        std::list<std::string> list;
        loadLuaFunction(pState, "", lua_gettop(pState), list);
        isInitSuccess = true;
    }else{
        LuaUtil::printLuaError(pState);
        isInitSuccess = false;
    }

}

LuaResolver::~LuaResolver() {
    LuaUtil::luaEnvironmentRelease(pState);
    pState = nullptr;
}


void LuaResolver::loadLuaFunction(lua_State *pState, const std::string &packageName, int index, std::list<std::string>& folderList) {
    if(!packageName.empty()) {
        lua_getglobal(pState, packageName.c_str());
    }

    lua_pushnil(pState);

    // lua_next弹出url表里的名称与方法
    while (lua_next(pState, index) != 0) {
        const char *key = lua_tostring(pState, -2);
        std::stringstream ss;
        if(!packageName.empty())
            ss << packageName << "/" << key;
        else
            ss << key;

        folderList.push_back(key);

        if (lua_isfunction(pState, -1)) {
            urlMethodMap.insert(std::make_pair(ss.str(), folderList));
            //std::cout << key << "is func:" << ss.str() << std::endl;
        }else if(lua_istable(pState, -1)) {
            loadLuaFunction(pState, ss.str(), lua_gettop(pState), folderList);
            //std::cout << key << "is table:" << ss.str() << std::endl;
        }
        lua_pop(pState, 1);
        folderList.pop_back();
    }
    lua_pop(pState, 1);
}

bool LuaResolver::handleRequest(const Request &req, Reply &rep) {
    if(!isInitSuccess) {
        rep = Reply::stockReply(Reply::internal_server_error);
        return false;
    }

    std::string reqPath;
    // 解析失败
    if (!UrlUtils::urlDecode(req.uri, reqPath)) {
        rep = Reply::stockReply(Reply::bad_request);
        return false;
    }

    // 去掉开头的斜杠
    if(reqPath.starts_with("/")) {
        reqPath = reqPath.substr(1);
    }


    std::string reqPurePath;
    // 请求url可能有text?xxx=xxxx
    auto pos = reqPath.find("?");
    if(pos != std::string::npos) {
        reqPurePath = reqPath.substr(0, pos);
    }else {
        reqPurePath = reqPath;
    }

    // 从urlMethodMap中找到对应的方法
    auto it = urlMethodMap.find(reqPurePath);
    if (it == urlMethodMap.end()) {
        return false;
    }

    lua_getglobal(pState, "url");
    auto folderList = it->second;
    // 把url各个字段压入栈中
    for (auto const &folder: folderList) {
        lua_getfield(pState, -1, folder.c_str());
    }
    // 字段错误
    if (!lua_isfunction(pState, -1)) {
        LogUtil::printError("Fail to get lua function " + it->first);
        rep = Reply::stockReply(Reply::internal_server_error);
        return false;
    }

    // 把request解析成表发送到lua层
    sendRequestToLua(pState, req);

    // 调用Lua方法
    if (lua_pcall(pState, 1, 1, 0) != LUA_OK) {
        LuaUtil::printLuaError(pState);
        rep = Reply::stockReply(Reply::internal_server_error);
        return false;
    }

    // 解析lua层返回的response到reply
    parseLuaReply(pState, rep);
    return true;
}

void LuaResolver::sendRequestToLua(lua_State *pState, const Request &req) {
    // request table
    lua_newtable(pState);

    // request.POST
    lua_pushstring(pState, "POST");
    lua_newtable(pState);

    // 遍历req.contentMap并push到POST表中
    for (auto const &[key, val]: req.bodyMap) {
        lua_pushstring(pState, key.c_str());
        lua_pushstring(pState, val.c_str());
        lua_settable(pState, -3);
    }
    // POST表压入request表
    lua_settable(pState, -3);

    // request.GET
    lua_pushstring(pState, "GET");
    lua_newtable(pState);
    // 遍历req.urlParamMap并push到GET表中
    for (auto const &[key, val]: req.urlParamMap) {
        lua_pushstring(pState, key.c_str());
        lua_pushstring(pState, val.c_str());
        lua_settable(pState, -3);
    }
    // GET表压入request表
    lua_settable(pState, -3);

    // request.HEADER
    lua_pushstring(pState, "HEADER");
    lua_newtable(pState);
    // 遍历req.headerMap并push到HEADER表中
    for (auto const &[key, val]: req.headerMap) {
        lua_pushstring(pState, key.c_str());
        lua_pushstring(pState, val.c_str());
        lua_settable(pState, -3);
    }
    // HEADER表压入request表
    lua_settable(pState, -3);

    // request.METHOD
    lua_pushstring(pState, "METHOD");
    lua_pushstring(pState, req.method.c_str());
    lua_settable(pState, -3);
}


void LuaResolver::parseLuaTable(lua_State *pState, rapidjson::Writer<rapidjson::StringBuffer> &writer, int index) {
    lua_pushnil(pState); // 第一个键
    writer.StartObject();
    while (lua_next(pState, index) != 0) {
        // 使用lua_tostring函数获取键和值
        writer.Key(lua_tostring(pState, -2));

        int arrayLen = 1;
        bool isTable = false, isArray = false;
        // 判断值的类型
        if (lua_istable(pState, -1)) {
            // lua_rawlen > 0就是数组
            arrayLen = lua_rawlen(pState, -1);
            isArray = arrayLen > 0;
            isTable = true;
        }

        // 纯表就纯解析
        if (isTable && !isArray) {
            // 如果值是一个表，递归调用自己
            parseLuaTable(pState, writer, lua_gettop(pState));
        } else {
            if(isArray) {
                // 是数组就让writer进入array状态
                writer.StartArray();
            }

            for(int i = 1; i <= arrayLen; ++i) {
                // 如果是数组就要先获取数组的值
                // 不是的话直接解析
                if(isArray) {
                    lua_rawgeti(pState, -1, i);
                }

                switch (lua_type(pState, -1)) {
                    case LUA_TSTRING:
                        writer.String(lua_tostring(pState, -1));
                        break;
                    case LUA_TBOOLEAN:
                        writer.Bool(lua_toboolean(pState, -1));
                        break;
                    case LUA_TNUMBER: {
                        double value = lua_tonumber(pState, -1);
                        if (ceil(value) == floor(value))
                            writer.Int((int) value);
                        else
                            writer.Double(lua_tonumber(pState, -1));
                        break;
                    }
                    case LUA_TTABLE:
                        parseLuaTable(pState, writer, lua_gettop(pState));
                        break;
                    default:
                        writer.Null();
                        break;
                }

                // 如果是数组就要弹出刚刚lua_rawgeti的
                if(isArray) {
                    lua_pop(pState, 1);
                }
            }

            // writer 退出数组模式
            if(isArray) {
                writer.EndArray();
            }
        }
        // 弹出值，保留键用于下一次迭代
        lua_pop(pState, 1);
    }
    writer.EndObject();
}


void LuaResolver::parseLuaReply(lua_State *pState, Reply &rep) {
    // 返回的都是metatable
    // 成功了metatable在栈顶
    if (!lua_getmetatable(pState, -1)) {
        LogUtil::printError("Can not parse reply from lua.");
        rep = Reply::stockReply(Reply::internal_server_error);
        return;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartObject();
    writer.Key("response");
    parseLuaTable(pState,  writer, -3);
    writer.EndObject();
    // 弹出返回的table
    lua_pop(pState, 1);

    // 设置json并返回
    Reply::setReply(rep, buffer.GetString(), "json");
}

