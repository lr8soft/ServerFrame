//
// Created by lrsoft on 2023/4/10.
//

#include <iostream>
#include <curl/curl.h>
#include "Server.h"
#include "Utils/LogUtil.h"
#include "Utils/HttpConnect.h"

Server* Server::pInst = nullptr;

void Server::init() {
    try{
        asio::ip::tcp::endpoint endPoint(asio::ip::tcp::v4(), 6780);
        pAcceptor = std::make_shared<AsioAcceptor>(ioService, endPoint);
        // 打印当前服务器地址
        std::cout << "Server address: " << pAcceptor->local_endpoint().address()
                  << ":" << pAcceptor->local_endpoint().port() << std::endl;
    }catch(std::exception& expt) {
        std::cout << "Server init failed " << expt.what() << std::endl;
    }
}

void Server::work() {
    while(!getIsTerminated()){
        timer.Tick();
        asio::ip::tcp::socket socket(ioService);
        pAcceptor->accept(socket);
        // 阻塞等待客户端连接，连接成功后返回socket, accept这个函数使用引用来调取socket.
        std::cout << "client try connect: " << socket.remote_endpoint().address() << std::endl;

        std::string msg;
        // 发点啥的
        socket.write_some(asio::buffer("HELLO FROM LT_LRSOFT!"));
        // 线程直接堵塞等他回复
        socket.read_some(asio::buffer(msg));

        std::cout << "client reply:" << msg << std::endl;
    }
}

void Server::finalize() {
    curl_global_cleanup();
}

bool Server::getIsTerminated() {
    return isTerminated;
}




