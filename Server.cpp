//
// Created by lrsoft on 2023/4/10.
//

#include <iostream>
#include <memory>
#include "Server.h"
#include "ConnManager.h"
#include "Utils/LogUtil.h"
#include "Core/RequestDispatcher.h"
#include "Resolvers/PostResolver.h"
#include "Resolvers/LocalResolver.h"

Server::Server(AsioService &service, const std::string &addr, const std::string &port)
        : _service(service), _acceptor(service), _nextSocket(service), _signals(service) {
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    doAwaitStop();

    AsioResolver resolver(_service);
    AsioEndPoint endPoint = *resolver.resolve({addr, port});
    _acceptor.open(endPoint.protocol());
    _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endPoint);
    _acceptor.listen();

    LogUtil::printInfo("ServerFrame started.");
    LogUtil::printInfo("Listen address: " + addr + ":" + port);

    this->doAccept();
}

void Server::init() {
    auto dispatcher = RequestDispatcher::getInstance();
    // POST请求
    dispatcher->addHandler("POST", std::make_shared<PostResolver>());
    // 允许GET statics文件夹下的内容
    dispatcher->addHandler("GET", std::make_shared<LocalResolver>("statics"));
}

void Server::start() {
    _service.run();
}

void Server::doAccept() {
    _acceptor.async_accept(_nextSocket, [this](const std::error_code& code){
        if(!_acceptor.is_open()) {
            LogUtil::printError("Acceptor is closed!");
            return;
        }

        LogUtil::printInfo("Socket connected with " + _nextSocket.remote_endpoint().address().to_string() + "  " + code.message());
        if(!code) {
            auto pConn = std::make_shared<Connection>(std::move(_nextSocket));
            ConnManager::getInstance()->startConn(pConn);
        }

        doAccept();
    });
}

void Server::doAwaitStop() {
    _signals.async_wait([this](const std::error_code& code, int val){
        _acceptor.close();
        ConnManager::getInstance()->stopAllConn();
        LogUtil::printInfo("All connection stop now.");
    });
}

