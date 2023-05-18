//
// Created by lrsoft on 2023/5/15.
//

#ifndef SERVERFRAME_LISTENERMANAGER_H
#define SERVERFRAME_LISTENERMANAGER_H

#include <list>
#include <string>
#include <map>
#include <memory>
#include <thread>
#include <lua.hpp>
class Listener;
class ListenerManager {
public:
    static ListenerManager* getInstance();

    void init();
    void start();
    void finalize();

private:
    ListenerManager()= default;

    void loadListeners(lua_State *pState);

private:
    static ListenerManager* pInstance;

    std::map<std::string, std::shared_ptr<Listener>> listenerMap;
    std::list<std::shared_ptr<std::thread>> threadList;

    bool isInitSuccess = false;
    lua_State *pState = nullptr;

};


#endif //SERVERFRAME_LISTENERMANAGER_H
