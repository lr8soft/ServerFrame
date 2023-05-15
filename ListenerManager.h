//
// Created by lrsoft on 2023/5/15.
//

#ifndef SERVERFRAME_LISTENERMANAGER_H
#define SERVERFRAME_LISTENERMANAGER_H

#include <list>
#include <string>
#include <map>
#include <memory>
#include <lua.hpp>
class Listener;
class ListenerManager {
public:
    static ListenerManager* getInstance();

    void init();
    void start();

private:
    ListenerManager()= default;

    void loadListeners(lua_State *pState);

private:
    static ListenerManager* pInstance;

    std::map<std::string, std::shared_ptr<Listener>> listenerMap;

    bool isInitSuccess = false;
    lua_State *pState;

};


#endif //SERVERFRAME_LISTENERMANAGER_H
