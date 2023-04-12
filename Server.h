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
    void start();
    // 处理接受连接后
    void handleAcceptAction(std::shared_ptr<AsioSocket> pSocket);
    void handleRequest(std::shared_ptr<AsioSocket> pSocket);
    void handleReceiveRequest(std::shared_ptr<AsioSocket> pSocket);
    void handleReleaseSocket(std::shared_ptr<AsioSocket> pSocket);
private:
    AsioService& _service;
    AsioAcceptor _acceptor;

    char buf[256];
    Timer timer;
};


#endif //SERVERFRAME_SERVER_H
