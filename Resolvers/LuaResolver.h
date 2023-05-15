//
// Created by lrsof on 2023/4/17.
//

#ifndef SERVERFRAME_LUARESOLVER_H
#define SERVERFRAME_LUARESOLVER_H
#include <map>
#include <string>
#include <list>
#include <lua.hpp>
#include <rapidjson/writer.h>
#include "IRequestSolver.h"

class LuaResolver: public IRequestSolver{
public:
    LuaResolver(lua_State* state);
    ~LuaResolver();

    bool handleRequest(const Request& req, Reply& rep);

private:
    /// 初始化时解析lua文件里的url表
    void loadLuaFunction(lua_State* pState, const std::string& packageName, int index, std::list<std::string>& folderList);

    /// 解析request对象并发送给lua层
    void sendRequestToLua(lua_State* pState, const Request& req);

    void parseLuaReply(lua_State* pState, Reply& rep);
private:
    std::map<std::string, std::list<std::string>> urlMethodMap;
    lua_State* pState;

    bool isInitSuccess = false;

};


#endif //SERVERFRAME_LUARESOLVER_H
