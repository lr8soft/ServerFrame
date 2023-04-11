//
// Created by lrsoft on 2023/4/10.
//

#ifndef SERVERFRAME_SERVER_H
#define SERVERFRAME_SERVER_H

#include <atomic>
#include <asio.hpp>
#include <memory>
#include "Utils/Timer.h"

typedef asio::ip::tcp::acceptor AsioAcceptor;
typedef asio::io_service AsioService;
typedef asio::ip::tcp::endpoint AsioEndPoint;
typedef asio::ip::tcp::socket AsioSocket;

class Server {
private:
    Timer timer;
    std::atomic<bool> isTerminated = false;
    AsioService& ioService;
    std::shared_ptr<AsioAcceptor> pAcceptor;

    void acceptHandler(int code, std::shared_ptr<AsioSocket> pSocket);
    void writeHandler(int code);
public:
    Server(AsioService& service): ioService(service){}

    void init();
    void work();
    void finalize();

    inline bool getIsTerminated();
};


#endif //SERVERFRAME_SERVER_H
