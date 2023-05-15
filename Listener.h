//
// Created by lrsoft on 2023/4/10.
//

#ifndef SERVERFRAME_LISTENER_H
#define SERVERFRAME_LISTENER_H

#include <atomic>
#include <memory>
#include <string>
#include <asio/ssl.hpp>
#include <lua.hpp>
#include "SimpleAsioDefine.h"
#include "Utils/Timer.h"


class Listener {
private:
    typedef asio::ip::tcp::resolver AsioResolver;
public:
    Listener(const std::string & name, lua_State* state);
    void init();
    void start();

private:
    // 异步新连接
    void doAccept();
    // 异步等信号
    void doAwaitStop();

private:
    bool _isHttps = false;
    std::string _appName;
    std::shared_ptr<asio::ssl::context> _pContext;
    std::shared_ptr<lua_State> _pState;

    AsioService _service;
    AsioAcceptor _acceptor;

    // 终止信息
    asio::signal_set _signals;
};


#endif //SERVERFRAME_LISTENER_H
