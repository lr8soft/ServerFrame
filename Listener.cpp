//
// Created by lrsoft on 2023/4/10.
//

#include <iostream>
#include <memory>

#include "Listener.h"
#include "ConnManager.h"
#include "Utils/LogUtil.h"
#include "Utils/PathUtils.h"
#include "Core/SSLConnection.h"
#include "Core/RequestDispatcher.h"
#include "Resolvers/LocalResolver.h"
#include "Resolvers/LuaResolver.h"
#include "Core/Connection.h"

Listener::Listener(const std::string & name, lua_State* state)
        : _appName(name), _pState(state), _acceptor(_service), _signals(_service) {
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    doAwaitStop();
}


void Listener::init() {
    // 从lua加载设置
    loadSettings();

    if(_isHttps) {
        _pContext = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
        // 加载https证书
        if(!_certPath.empty() && !_keyPath.empty()) {
            _pContext->use_certificate_chain_file(PathUtils::getRealPath(_certPath));
            _pContext->use_private_key_file(PathUtils::getRealPath(_keyPath), asio::ssl::context::pem);
        }else{
            LogUtil::printError("Cert path or key path not set in " + _appName + "!");
        }
    }

    if(_port != -1 && !_address.empty()) {
        AsioResolver resolver(_service);
        AsioEndPoint endPoint = *resolver.resolve({_address.c_str(), std::to_string(_port).c_str()});
        _acceptor.open(endPoint.protocol());
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(endPoint);
        _acceptor.listen();
    }else{
        LogUtil::printError("Port or address not set in " + _appName + "!");
    }


    auto dispatcher = RequestDispatcher::getInstance();
    // POST请求
    dispatcher->addHandler(std::make_shared<LuaResolver>(_pState));

    if(!_staticFolder.empty()) {
        // 允许GET statics文件夹下的内容
        dispatcher->addHandler(std::make_shared<LocalResolver>(_staticFolder));
    }

    this->doAccept();

}

void Listener::loadSettings() {
    // 遍历app的设置
    lua_pushnil(_pState);

    int index = lua_gettop(_pState);
    std::cout << _appName <<" loadSettings index:" << index << std::endl;

    while (lua_next(_pState, -2) != 0) {
        const char *key = lua_tostring(_pState, -2);
        //std::cout << _appName << " loadSettings key:" << key << std::endl;
        if(strcmp(key, "port") == 0) {
            _port = lua_tointeger(_pState, -1);
        }else if(strcmp(key, "address") == 0) {
            _address = lua_tostring(_pState, -1);
        }else if(strcmp(key, "isHttps") == 0) {
            _isHttps = lua_toboolean(_pState, -1);
        }else if(strcmp(key, "static") == 0) {
            _staticFolder = lua_tostring(_pState, -1);
        }else if(strcmp(key, "cert") == 0) {
            // 解析cert表
            lua_pushnil(_pState);
            while (lua_next(_pState, -2) != 0) {
                const char *certKey = lua_tostring(_pState, -2);
                if(strcmp(certKey, "cert") == 0) {
                    // 证书文件
                    _certPath = lua_tostring(_pState, -1);
                }else if(strcmp(certKey, "key") == 0) {
                    // 密钥文件
                    _keyPath = lua_tostring(_pState, -1);
                }
                lua_pop(_pState, 1);
            }
            // 不pop掉nil，保留在栈顶允许继续遍历
            //lua_pop(_pState, 1);
        }
        lua_pop(_pState, 1);
    }
    // 保留nil
    //lua_pop(_pState, 1);
}


void Listener::start() {
    _service.run();
}

void Listener::doAccept() {
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

void Listener::doAwaitStop() {
    _signals.async_wait([this](const std::error_code& code, int val){
        _acceptor.close();
        ConnManager::getInstance()->stopAllConn();
        LogUtil::printInfo("All connection stop now.");
    });
}


