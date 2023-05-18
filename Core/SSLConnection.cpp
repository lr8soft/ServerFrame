//
// Created by lrsof on 2023/5/10.
//

#include <iostream>
#include "SSLConnection.h"
#include "RequestDispatcher.h"
#include "../ConnManager.h"
#include "../Utils/LogUtil.h"

void SSLConnection::start() {
    this->doRead();
}

void SSLConnection::doRead() {
    _socket.async_read_some(asio::buffer(_buffer),
        [this](const std::error_code &code, std::size_t bytesCount) {
        onReadAction(code, bytesCount, _buffer, _bufferStream, _request, _reply, _parser);
    });
}

void SSLConnection::doWrite() {
    asio::async_write(_socket, _reply.toBuffers(),
        [this](const std::error_code &code, std::size_t size) {
            // 优雅地关闭连接
            if (!code) {
                std::error_code result;
                _socket.shutdown(result);
            }

            if (code != asio::error::operation_aborted) {
                ConnManager::getInstance()->stopConn(shared_from_this());
            }
        });
}

void SSLConnection::stop() {
    std::error_code result;
    _socket.shutdown(result);
}