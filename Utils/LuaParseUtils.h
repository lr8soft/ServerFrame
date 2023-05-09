//
// Created by lrsof on 2023/4/28.
//

#ifndef SERVERFRAME_LUAPARSEUTILS_H
#define SERVERFRAME_LUAPARSEUTILS_H

#include <lua.h>
#include <rapidjson/writer.h>

class LuaParseUtils {
public:
    static void parseLuaTable(lua_State *pState, rapidjson::Writer<rapidjson::StringBuffer> &writer, int index);

};


#endif //SERVERFRAME_LUAPARSEUTILS_H
