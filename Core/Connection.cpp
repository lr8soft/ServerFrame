//
// Created by lrsoft on 2023/4/12.
//

#include <iostream>
#include "Connection.h"
#include "RequestDispatcher.h"
#include "../ConnManager.h"
#include "../Utils/LogUtil.h"

void Connection::start() {
    this->doRead();
}

void Connection::doRead() {
    _socket.async_read_some(asio::buffer(_buffer),
        [this](const std::error_code & code, std::size_t bytesCount){
        onReadAction(code, bytesCount, _buffer, _bufferStream, _request, _reply, _parser);
    });
}

void Connection::doWrite() {
    asio::async_write(_socket, _reply.toBuffers(),
        [this](const std::error_code &code, std::size_t size) {
        // 优雅地关闭连接捏
        if (!code) {
            std::error_code result;
            _socket.shutdown(asio::ip::tcp::socket::shutdown_both, result);
        }

        if(code != asio::error::operation_aborted) {
            ConnManager::getInstance()->stopConn(shared_from_this());
        }
    });
}

void Connection::stop() {
    _socket.close();
}


