//
// Created by lrsoft on 2023/5/10.
//

#ifndef SERVERFRAME_ICONN_H
#define SERVERFRAME_ICONN_H

#include <array>
#include <memory>
#include <sstream>

class Request;
class RequestParser;
class Reply;

class IConn: public std::enable_shared_from_this<IConn> {
public:
    IConn(std::string appName) : _appName(appName) {}

    virtual void start() = 0;
    virtual void stop() = 0;

    std::string getAppName() {
        return _appName;
    }


protected:
    void onReadAction(const std::error_code & code, std::size_t bytesCount, std::array<char, 8192> & _buffer,
                      std::stringstream & _bufferStream, Request & _request, Reply & reply, RequestParser & _parser);

    // doRead调用onReadAction
    virtual void doRead() = 0;
    virtual void doWrite() = 0;

private:
    std::string _appName;
};

#endif //SERVERFRAME_ICONN_H
