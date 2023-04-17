//
// Created by lrsof on 2023/4/17.
//

#ifndef SERVERFRAME_LUARESOLVER_H
#define SERVERFRAME_LUARESOLVER_H
#include <map>
#include <string>
#include <lua.hpp>
#include "IRequestSolver.h"

class LuaResolver: public IRequestSolver{
public:
    LuaResolver();
    ~LuaResolver();

    bool handleRequest(const Request& req, Reply& rep);

private:
    std::string parserAnyValue(lua_State* state, int index);

private:
    std::map<std::string, int> urlMethodMap;
    lua_State* pState;
};


#endif //SERVERFRAME_LUARESOLVER_H
