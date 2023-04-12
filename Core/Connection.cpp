//
// Created by lrsoft on 2023/4/12.
//

#include <iostream>
#include "Connection.h"

void Connection::start() {
    this->doRead();
}

void Connection::doRead() {
    _socket->async_read_some(asio::buffer(_buffer),
        [this](const std::error_code & code, std::size_t bytesCount){
        if(!code) {
            std::cerr << "Server doRead fail with code " << code << std::endl;
            return;
        }

        RequestParser::ResultEnum result;
        std::tie(result, std::ignore) = _parser.parse(
                _request, _buffer.data(), _buffer.data() + bytesCount);  // +bytesCount计算iterend
        if(result == RequestParser::good) {
           // _parser
        }
    });
}

void Connection::stop() {
    _socket->close();
}
