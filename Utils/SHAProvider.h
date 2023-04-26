//
// Created by lrsoft on 2023/4/26.
//

#ifndef SERVERFRAME_SHAPROVIDER_H
#define SERVERFRAME_SHAPROVIDER_H

#include <string>
#include <openssl/sha.h>
#include <lua.h>

class SHAProvider {
public:
    static std::string encryptBySHA256(std::string& info);

    static int luaEncryptBySHA256(lua_State *pState);
    static int luaLibInit(lua_State *pState);
};


#endif //SERVERFRAME_SHAPROVIDER_H
