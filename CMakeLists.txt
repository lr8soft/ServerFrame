cmake_minimum_required(VERSION 3.25)
project(ServerFrame)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServerFrame main.cpp Utils/HttpConnect.cpp Utils/HttpConnect.h)

find_package(CURL CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE CURL::libcurl)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(ServerFrame PRIVATE JsonCpp::JsonCpp)