//
// Created by lrsoft on 2023/4/26.
//

#include <sstream>
#include <lua.hpp>
#include <iostream>
#include <iomanip>
#include <openssl/hmac.h>
#include "SHAProvider.h"

std::string SHAProvider::encryptBySHA256(std::string &info) {
    SHA256_CTX sha256;
    std::stringstream result;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, info.c_str(), info.length());
    SHA256_Final(hash, &sha256);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        result << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return result.str();
}


std::string SHAProvider::encryptByHMAC256(std::string &info, std::string &key) {
    std::stringstream result;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    HMAC_CTX *hmac = HMAC_CTX_new();
    HMAC_Init_ex(hmac, key.c_str(), key.length(), EVP_sha256(), NULL);
    HMAC_Update(hmac, (unsigned char*)info.c_str(), info.length());
    HMAC_Final(hmac, hash, NULL);
    HMAC_CTX_free(hmac);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        result << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return result.str();
}


int SHAProvider::luaEncryptBySHA256(lua_State *pState) {
    // 先检测是否string，再读取从lua返回的string参数
    if(!lua_isstring(pState, -1)) {
        lua_pushstring(pState, "SHAProvider::luaEncryptBySHA256: Invalid parameter type.");
        lua_error(pState);
        return 0;
    }

    std::string info = lua_tostring(pState, -1);
    std::string result = encryptBySHA256(info);
    lua_pushstring(pState, result.c_str());
    return 1;
}


int SHAProvider::luaEncryptByHMAC256(lua_State *pState) {
    if(!lua_isstring(pState, -2) || !lua_isstring(pState, -1)) {
        lua_pushstring(pState, "SHAProvider::luaEncryptByHMAC256: Invalid parameter type.");
        lua_error(pState);
        return 0;
    }

    std::string info = lua_tostring(pState, -2);
    std::string key = lua_tostring(pState, -1);
    std::string result = encryptByHMAC256(info, key);
    lua_pushstring(pState, result.c_str());
    return 1;
}


int SHAProvider::luaLibInit(lua_State *pState) {
    luaL_newmetatable(pState, "SHA");

    lua_pushvalue(pState, -1);
    lua_setfield(pState, -2, "__index");

    static const struct luaL_Reg objectlib_m[] = {
        {NULL, NULL}
    };

    static const struct luaL_Reg objectlib_f[] = {
        {"sha256", luaEncryptBySHA256},
        {"hmac256", luaEncryptByHMAC256},
        {NULL, NULL}
    };

    luaL_setfuncs(pState, objectlib_m, 0);
    luaL_newlib(pState, objectlib_f);

    return 1;
}