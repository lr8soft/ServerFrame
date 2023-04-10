//
// Created by lrsoft on 2023/4/10.
//

#include <iostream>
#include <curl/curl.h>
#include <asio.hpp>
#include "Application.h"
#include "Utils/LogUtil.h"
#include "Utils/HttpConnect.h"

Application* Application::pInstance = nullptr;

Application *Application::getInstance() {
    if(pInstance == nullptr)
        pInstance =  new Application;
    return nullptr;
}

void Application::init() {
    curl_global_init(CURL_GLOBAL_ALL);

    LogUtil::printInfo(curl_version());

    asio::io_service ios;
    // 具体的服务器地址与端口
    asio::ip::tcp::endpoint endpotion(asio::ip::tcp::v4(), 13695);
    // 创建acceptor对象，当前的IPV4作为服务器地址(127.0.0.1 || 0.0.0.0)，接受端口13695的消息.
    asio::ip::tcp::acceptor acceptor(ios, endpotion);
    // 打印当前服务器地址
    std::cout << "addr: " << acceptor.local_endpoint().address() << std::endl;
    // 打印当前服务器端口
    std::cout << "port: " << acceptor.local_endpoint().port() << std::endl;
}

void Application::work() {
    while(!getIsTerminated()){
        timer.Tick();
        //LogUtil::printWarn(std::to_string(timer.getAccumlateTime()));
    }
}

void Application::finalize() {
    curl_global_cleanup();
}

bool Application::getIsTerminated() {
    return isTerminated;
}
