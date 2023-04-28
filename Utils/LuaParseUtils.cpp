//
// Created by lrsof on 2023/4/28.
//

#include <cmath>
#include <string>
#include <lua.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include "LuaParseUtils.h"

void LuaParseUtils::parseLuaTable(lua_State *pState, rapidjson::Writer<rapidjson::StringBuffer> &writer, int index) {
    lua_pushnil(pState); // 第一个键
    writer.StartObject();
    while (lua_next(pState, index) != 0) {
        // 使用lua_tostring函数获取键和值
        writer.Key(lua_tostring(pState, -2));

        int arrayLen = 1;
        bool isTable = false, isArray = false;
        // 判断值的类型
        if (lua_istable(pState, -1)) {
            // lua_rawlen > 0就是数组
            arrayLen = lua_rawlen(pState, -1);
            isArray = arrayLen > 0;
            isTable = true;
        }

        // 纯表就纯解析
        if (isTable && !isArray) {
            // 如果值是一个表，递归调用自己
            parseLuaTable(pState, writer, lua_gettop(pState));
        } else {
            if(isArray) {
                // 是数组就让writer进入array状态
                writer.StartArray();
            }

            for(int i = 1; i <= arrayLen; ++i) {
                // 如果是数组就要先获取数组的值
                // 不是的话直接解析
                if(isArray) {
                    lua_rawgeti(pState, -1, i);
                }

                switch (lua_type(pState, -1)) {
                    case LUA_TSTRING:
                        writer.String(lua_tostring(pState, -1));
                        break;
                    case LUA_TBOOLEAN:
                        writer.Bool(lua_toboolean(pState, -1));
                        break;
                    case LUA_TNUMBER: {
                        double value = lua_tonumber(pState, -1);
                        if (ceil(value) == floor(value))
                            writer.Int((int) value);
                        else
                            writer.Double(lua_tonumber(pState, -1));
                        break;
                    }
                    case LUA_TTABLE:
                        parseLuaTable(pState, writer, lua_gettop(pState));
                        break;
                    default:
                        writer.Null();
                        break;
                }

                // 如果是数组就要弹出刚刚lua_rawgeti的
                if(isArray) {
                    lua_pop(pState, 1);
                }
            }

            // writer 退出数组模式
            if(isArray) {
                writer.EndArray();
            }
        }
        // 弹出值，保留键用于下一次迭代
        lua_pop(pState, 1);
    }
    writer.EndObject();
}
