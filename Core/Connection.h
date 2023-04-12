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
#include "RequestHandler.h"
#include "Reply.h"

class Connection: public std::enable_shared_from_this<Connection> {
private:
    std::array<char, 8192> _buffer;
    AsioSocket _socket;

    Request _request;
    RequestParser _parser;
    RequestHandler& _requestHandler;

    Reply _reply;

private:
    void doRead();
    void doWrite();
public:
    explicit Connection(AsioSocket socket, RequestHandler &handler)
            : _socket(std::move(socket)), _requestHandler(handler) {};

    void start();
    void stop();
};


#endif //SERVERFRAME_CONNECTION_H
