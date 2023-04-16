//
// Created by lrsoft on 2023/4/16.
//

#include "LuaUtil.h"

lua_State *LuaUtil::getNewState() {
    lua_State *luaState = luaL_newstate();
    static const luaL_Reg lualibs[] = {
            {"base", luaopen_base},
            {"math", luaopen_math},
            {"io",   luaopen_io},
            //{ "NewHorizon", GameObjectBinder::luaOpenGameObject},
            {NULL, NULL}
    };

    const luaL_Reg *lib = lualibs;
    for (; lib->func != nullptr; lib++) {
        luaL_requiref(luaState, lib->name, lib->func, 1);
        lua_pop(luaState, 1);
    }
    return luaState;
}

void LuaUtil::luaEnvironmentRelease(lua_State *pState) {
    lua_close(pState);
}
