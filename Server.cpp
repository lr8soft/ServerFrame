//
// Created by lrsoft on 2023/4/10.
//

#include <iostream>
#include <curl/curl.h>
#include "Server.h"
#include "Utils/LogUtil.h"
#include "Utils/HttpConnect.h"

void Server::init() {
    try{
        AsioEndPoint endPoint(asio::ip::tcp::v4(), 6780);
        pAcceptor = std::make_shared<AsioAcceptor>(ioService, endPoint);
        // 打印当前服务器地址
        std::cout << "Server address: " << pAcceptor->local_endpoint().address()
                  << ":" << pAcceptor->local_endpoint().port() << std::endl;
    }catch(std::exception& expt) {
        std::cout << "Server init failed " << expt.what() << std::endl;
    }
}

void Server::work() {
    auto pSocket = std::make_shared<AsioSocket>(ioService);
    pAcceptor->async_accept(*pSocket, std::bind(&Server::acceptHandler, this, 1, pSocket));
}

void Server::finalize() {
    curl_global_cleanup();
}

bool Server::getIsTerminated() {
    return isTerminated;
}

void Server::acceptHandler(int code, std::shared_ptr<AsioSocket> pSocket) {
    if(!code) {
        std::cerr << "Accpet handle fail CODE" << code << std::endl;
        return;
    }
    std::cout << "Client: " << pSocket->remote_endpoint().address() << std::endl;
    pSocket->async_write_some(asio::buffer("HELLO WORLD! FROM SERVER FLAME"),
                              std::bind(&Server::writeHandler, this, 1));
    this->work();
}

void Server::writeHandler(int code) {
    std::cout << "Send message to client CODE" << code << std::endl;
}




