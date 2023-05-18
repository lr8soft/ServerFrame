//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_CONNECTION_H
#define SERVERFRAME_CONNECTION_H

#include "../SimpleAsioDefine.h"
#include "IConn.h"
#include <memory>
#include <sstream>

#include "Request.hpp"
#include "RequestParser.hpp"
#include "Reply.h"

class Connection: public IConn {
private:
    std::array<char, 8192> _buffer;
    std::stringstream _bufferStream;
    AsioSocket _socket;

    Request _request;
    RequestParser _parser;

    Reply _reply;

protected:
    void doRead() override;
    void doWrite() override;

public:
    Connection(const std::string& appName, AsioSocket socket)
            : IConn(appName), _socket(std::move(socket)){ }

    void start() override;
    void stop() override;
};


#endif //SERVERFRAME_CONNECTION_H
