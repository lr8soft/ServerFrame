//
// Created by lrsoft on 2023/4/16.
//

#ifndef SERVERFRAME_LUAUTIL_H
#define SERVERFRAME_LUAUTIL_H

#include <lua.hpp>

class LuaUtil {
public:
    static lua_State* getNewState();
    static void luaEnvironmentRelease(lua_State* pState);

};

#endif //SERVERFRAME_LUAUTIL_H
