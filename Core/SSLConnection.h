//
// Created by lrsof on 2023/5/10.
//

#ifndef SERVERFRAME_SSLCONNECTION_H
#define SERVERFRAME_SSLCONNECTION_H

#include "../SimpleAsioDefine.h"
#include "IConn.h"
#include <array>
#include <memory>
#include <sstream>
#include <asio/ssl.hpp>

#include "Request.hpp"
#include "RequestParser.hpp"
#include "Reply.h"

class SSLConnection: public IConn {
private:
    std::array<char, 8192> _buffer;
    std::stringstream _bufferStream;
    asio::ssl::stream<AsioSocket> _socket;

    Request _request;
    RequestParser _parser;

    Reply _reply;


private:
    void doRead();
    void doWrite();
public:
    SSLConnection(const std::string& appName, asio::ssl::stream<AsioSocket> socket)
        : IConn(appName), _socket(std::move(socket)){}

    void start();
    void stop();
};


#endif //SERVERFRAME_SSLCONNECTION_H
