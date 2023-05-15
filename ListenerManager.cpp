//
// Created by lrsoft on 2023/5/15.
//

#include "ListenerManager.h"
#include "Listener.h"
#include "Utils/LuaUtil.h"
#include "Utils/LogUtil.h"

ListenerManager *ListenerManager::pInstance = nullptr;
ListenerManager *ListenerManager::getInstance() {
    return nullptr;
}

void ListenerManager::init() {
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
    // 根据路径加载lua脚本
    if(luaL_dofile(pState, path) == LUA_OK) {
        lua_getglobal(pState, "manage");
        lua_getfield(pState, -1, "app");

        // 解析lua脚本中的app表
        loadListeners(pState);

        isInitSuccess = true;
    }else{
        LuaUtil::printLuaError(pState);
        isInitSuccess = false;
    }

}

void ListenerManager::loadListeners(lua_State *pState) {
    lua_pushnil(pState);
    // lua_next弹出url表里的名称与方法
    while (lua_next(pState, lua_gettop(pState)) != 0) {
        const char *key = lua_tostring(pState, -2);
        // 判断是否为table
        if(lua_istable(pState, -1)) {
            // 创建监听器并初始化
            auto pListener = std::make_shared<Listener>(key, lua_newthread(pState));
            pListener->init();
            listenerMap.insert(std::make_pair(key, pListener));
        }
    }
    lua_pop(pState, 1);
}

void ListenerManager::start() {
    if(!isInitSuccess) {
        LogUtil::printError("ListenerManager init failed!");
        return;
    }
    // 启动监听器
    for(auto& item : listenerMap) {
        item.second->start();
    }
}


