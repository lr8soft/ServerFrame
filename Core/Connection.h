//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_CONNECTION_H
#define SERVERFRAME_CONNECTION_H

#include "../SimpleAsioDefine.h"
#include <array>
#include <memory>

#include "Request.hpp"
#include "RequestParser.hpp"

class Connection {
private:
    std::array<char, 8192> _buffer;
    std::shared_ptr<AsioSocket> _socket;

    Request _request;
    RequestParser _parser;

private:
    void doRead();

public:
    explicit Connection(std::shared_ptr<AsioSocket> ptr): _socket(ptr) {};

    void start();
    void stop();
};


#endif //SERVERFRAME_CONNECTION_H
