//
// Created by lrsoft on 2023/4/10.
//

#ifndef SERVERFRAME_SERVER_H
#define SERVERFRAME_SERVER_H

#include <atomic>
#include <asio.hpp>
#include <memory>
#include "Utils/Timer.h"
class Server {
private:
    typedef asio::ip::tcp::acceptor AsioAcceptor;
    typedef asio::io_service AsioService;

    Timer timer;

    std::atomic<bool> isTerminated = false;

    AsioService ioService;
    std::shared_ptr<AsioAcceptor> pAcceptor;

    static Server* pInst;
    Server(){}
public:
    static Server* getInstance() {
        if(pInst == nullptr)
            pInst = new Server;
        return pInst;
    }
    void init();
    void work();
    void finalize();

    inline bool getIsTerminated();
};


#endif //SERVERFRAME_SERVER_H
