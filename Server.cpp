//
// Created by lrsoft on 2023/4/10.
//

#include <iostream>
#include <memory>

#include "Server.h"
#include "ConnManager.h"
#include "Utils/LogUtil.h"
#include "Core/SSLConnection.h"
#include "Core/RequestDispatcher.h"
#include "Resolvers/LocalResolver.h"
#include "Resolvers/LuaResolver.h"
#include "Core/Connection.h"

Server::Server(bool isHttps, const std::string &addr, const std::string &port)
        : _isHttps(isHttps), _acceptor(_service), _signals(_service) {
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    doAwaitStop();

    if(isHttps) {
        _pContext = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
#ifdef _DEBUG
        _pContext->use_certificate_chain_file("../cert/localhost.crt");
        _pContext->use_private_key_file("../cert/localhost.key", asio::ssl::context::pem);
#else
        _pContext->use_certificate_chain_file("./cert/localhost.crt");
        _pContext->use_private_key_file("./cert/localhost.key", asio::ssl::context::pem);
#endif
    }

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
    dispatcher->addHandler(std::make_shared<LuaResolver>());
    // 允许GET statics文件夹下的内容
    dispatcher->addHandler(std::make_shared<LocalResolver>("statics"));
}

void Server::start() {
    _service.run();
}

void Server::doAccept() {
    if(!_isHttps) {
        auto pSocket = std::make_shared<AsioSocket>(_service);
        _acceptor.async_accept(*pSocket, [this, pSocket](const std::error_code &code) {
            if (!_acceptor.is_open()) {
                LogUtil::printError("Acceptor is closed!");
                return;
            }

            if (!code) {
                auto pConn = std::make_shared<Connection>(std::move(*pSocket));
                ConnManager::getInstance()->startConn(pConn);
            }

            doAccept();
        });
    }else{
        // 记录_pNextSSLSocket->lowest_layer
        auto pSocket = std::make_shared<asio::ssl::stream<AsioSocket>>(_service, *_pContext);
        _acceptor.async_accept(pSocket->lowest_layer(), [this, pSocket](const std::error_code &code) {
            if (!_acceptor.is_open()) {
                LogUtil::printError("Acceptor is closed!");
                return;
            }

            if (!code) {
                pSocket->async_handshake(asio::ssl::stream_base::server, [pSocket](const std::error_code &code) {
                    if (!code) {
                        auto pConn = std::make_shared<SSLConnection>(std::move(*pSocket));
                        ConnManager::getInstance()->startConn(pConn);
                    }
                });
            }

            doAccept();
        });
    }

}

void Server::doAwaitStop() {
    _signals.async_wait([this](const std::error_code& code, int val){
        _acceptor.close();
        ConnManager::getInstance()->stopAllConn();
        LogUtil::printInfo("All connection stop now.");
    });
}

