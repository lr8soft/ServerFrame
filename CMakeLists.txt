cmake_minimum_required(VERSION 3.25)
project(ServerFrame)

set(CMAKE_CXX_STANDARD 20)

add_executable(ServerFrame main.cpp Utils/HttpConnect.cpp Utils/HttpConnect.h Server.cpp Server.h Utils/Timer.cpp Utils/Timer.h Utils/LogUtil.cpp Utils/LogUtil.h SimpleAsioDefine.h ConnManager.cpp ConnManager.h ConnManager.h Core/Connection.cpp Core/Connection.h Core/RequestParser.cpp Core/RequestParser.hpp Core/Request.hpp Core/Request.hpp Resolvers/LocalResolver.cpp Resolvers/LocalResolver.h Resolvers/LocalResolver.h Core/Reply.cpp Core/Reply.h Core/Reply.h Core/Header.h Core/Header.h Utils/MimeUtils.cpp Utils/MimeUtils.h Core/RequestDispatcher.cpp Core/RequestDispatcher.h Utils/UrlUtils.cpp Utils/UrlUtils.h Resolvers/IRequestSolver.h Core/ContentParser.hpp Core/ContentParser.hpp Core/X3WFormParser.cpp Core/X3WFormParser.h Core/X3WFormParser.h Core/MultiFormParser.cpp Core/MultiFormParser.h Utils/LuaUtil.cpp Utils/LuaUtil.h Resolvers/LuaResolver.cpp Resolvers/LuaResolver.h Resolvers/LuaResolver.h)

find_package(CURL CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE CURL::libcurl)

find_package(RapidJSON CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE rapidjson)

find_package(asio CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE asio::asio)
# asio需要链接对应lib
if(WIN32)
    target_link_libraries(ServerFrame PRIVATE wsock32 ws2_32)
elseif(UNIX)
    target_link_libraries(ServerFrame PRIVATE pthread)
endif()

find_package(lua REQUIRED)
target_link_libraries(ServerFrame PRIVATE lua)

find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE sqlite_orm::sqlite_orm)

# 添加debug宏
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-D_DEBUG)
endif ()