//
// Created by lrsoft on 2023/5/15.
//

#ifndef SERVERFRAME_LISTENERMANAGER_H
#define SERVERFRAME_LISTENERMANAGER_H

#include <list>
#include <string>
#include <lua.hpp>

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

    std::list<std::string> appList;
    bool isInitSuccess = false;
    lua_State *pState;

};


#endif //SERVERFRAME_LISTENERMANAGER_H
