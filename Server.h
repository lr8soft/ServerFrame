//
// Created by lrsoft on 2023/4/10.
//

#ifndef SERVERFRAME_SERVER_H
#define SERVERFRAME_SERVER_H

#include <atomic>
#include <memory>
#include <string>
#include "SimpleAsioDefine.h"
#include "Utils/Timer.h"


class Server {
private:
    typedef asio::ip::tcp::resolver AsioResolver;
public:
    Server(AsioService& service, const std::string & addr = "0.0.0.0", const std::string & port = "6780");
    void init();
    void start();

private:
    // 异步新连接
    void doAccept();
    // 异步等信号
    void doAwaitStop();

private:
    AsioService& _service;
    AsioAcceptor _acceptor;

    // 终止信息
    asio::signal_set _signals;
    // 下个连接的socket
    AsioSocket _nextSocket;
};


#endif //SERVERFRAME_SERVER_H
