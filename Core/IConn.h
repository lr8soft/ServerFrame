//
// Created by lrsof on 2023/5/10.
//

#ifndef SERVERFRAME_ICONN_H
#define SERVERFRAME_ICONN_H
#include <memory>
#include <string>
class IConn: public std::enable_shared_from_this<IConn> {
public:
    virtual void start() = 0;
    virtual void stop() = 0;

    std::string getAppName() {
        return _appName;
    }

    IConn(std::string appName) : _appName(appName) {}
protected:
    std::string _appName;
};

#endif //SERVERFRAME_ICONN_H
