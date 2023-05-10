//
// Created by lrsof on 2023/5/10.
//

#ifndef SERVERFRAME_ICONN_H
#define SERVERFRAME_ICONN_H
#include <memory>
class IConn: public std::enable_shared_from_this<IConn> {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif //SERVERFRAME_ICONN_H
