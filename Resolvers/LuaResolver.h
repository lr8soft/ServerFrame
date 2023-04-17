//
// Created by lrsof on 2023/4/17.
//

#ifndef SERVERFRAME_LUARESOLVER_H
#define SERVERFRAME_LUARESOLVER_H
#include <map>
#include <string>
#include <lua.hpp>
#include <rapidjson/writer.h>
#include "IRequestSolver.h"

class LuaResolver: public IRequestSolver{
public:
    LuaResolver();
    ~LuaResolver();

    bool handleRequest(const Request& req, Reply& rep);

private:
    /// 解析栈顶的lua表内容并写入至writer
    void parseLuaTable(lua_State* pState, rapidjson::Writer<rapidjson::StringBuffer> &writer, int index);
private:
    std::map<std::string, int> urlMethodMap;
    lua_State* pState;

};


#endif //SERVERFRAME_LUARESOLVER_H
