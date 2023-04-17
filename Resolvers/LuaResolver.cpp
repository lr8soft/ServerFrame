//
// Created by lrsof on 2023/4/17.
//

#include <lua.hpp>
#include "LuaResolver.h"
#include "../Core/Request.hpp"
#include "../Core/Reply.h"
#include "../Utils/LuaUtil.h"
#include "../Utils/LogUtil.h"
#include "../Utils/UrlUtils.h"

LuaResolver::LuaResolver() {
    pState = LuaUtil::getNewState();
    /*if(luaL_loadfilex(pState, "scripts/manage.lua", NULL) != LUA_OK) {
        LogUtil::printError("Fail to load manage.lua");
        return;
    }*/
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
            std::cout << " key:" << key << std::endl;
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
    if (it != urlMethodMap.end()) {
        lua_getglobal(pState, "url");
        lua_pushstring(pState, it->first.c_str());
        lua_gettable(pState, -2);
        if (lua_isfunction(pState, -1)) {
            lua_newtable(pState);
            int tableIndex = lua_gettop(pState);
            // 遍历req.contentMap并push到Lua表中
            for (auto const& [key, val] : req.bodyMap) {
                lua_pushstring(pState, key.c_str());
                lua_pushstring(pState, val.c_str());
                lua_settable(pState, tableIndex);
            }

            if(lua_pcall(pState, 1, 1, 0) != LUA_OK) {
                LogUtil::printError("Fail to call lua function");
                return false;
            }

            //std::cout << "work2" << std::endl;
            //const char *result = lua_tostring(pState, -1);
            //Reply::setReply(rep, result, "json");
            return true;

        }

    }

    return false;
}




