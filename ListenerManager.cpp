//
// Created by lrsoft on 2023/5/15.
//

#include "ListenerManager.h"
#include "Listener.h"
#include "Utils/LuaUtil.h"
#include "Utils/LogUtil.h"
#include "Utils/PathUtils.h"

ListenerManager *ListenerManager::pInstance = nullptr;
ListenerManager *ListenerManager::getInstance() {
    if(pInstance == nullptr)
        pInstance = new ListenerManager;
    return pInstance;
}

void ListenerManager::init() {
    std::string path = PathUtils::getRealPath("scripts/manage.lua");
    pState = LuaUtil::getNewState();
    // 根据路径加载lua脚本
    if(luaL_dofile(pState, path.c_str()) == LUA_OK) {
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
    while (lua_next(pState, -2) != 0) {
        std::string key = lua_tostring(pState, -2);
        //std::cout << "Load "<< key <<" Listener" << std::endl;
        // 判断是否为table
        if(!key.empty() && lua_istable(pState, -1)) {
            // 创建监听器并初始化
            auto pListener = std::make_shared<Listener>(key, pState);
            pListener->init();
            listenerMap.insert(std::make_pair(key, pListener));
        }
        lua_pop(pState, 1);
    }
    lua_pop(pState, 1);
}

void ListenerManager::start() {
    if(!isInitSuccess) {
        LogUtil::printError("ListenerManager init failed!");
        return;
    }
    // 启动监听器线程
    for(auto& item : listenerMap) {
        // 新建线程并启动
        auto pThread = std::make_shared<std::thread>(&Listener::start, item.second);
        threadList.push_back(pThread);
        pThread->detach();
    }

    // 等待所有子线程joinable
    while(true) {
        bool isAllJoinable = true;
        for(auto& item : threadList) {
            if(!item->joinable())
                isAllJoinable = false;
        }
        // 所有子线程结束
        if(isAllJoinable)
            break;
    }

}


