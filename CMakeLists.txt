cmake_minimum_required(VERSION 3.25)
project(ServerFrame)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServerFrame main.cpp Utils/HttpConnect.cpp Utils/HttpConnect.h Server.cpp Server.h Utils/Timer.cpp Utils/Timer.h Utils/LogUtil.cpp Utils/LogUtil.h)

find_package(CURL CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE CURL::libcurl)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE JsonCpp::JsonCpp)

find_package(asio CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE asio::asio)
# asio需要链接对应lib
if(WIN32)
    target_link_libraries(ServerFrame PRIVATE ws2_32)
elseif(UNIX)
    target_link_libraries(ServerFrame PRIVATE pthread boost_system)
endif()