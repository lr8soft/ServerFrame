//
// Created by lrsof on 2023/5/10.
//

#ifndef SERVERFRAME_ICONN_H
#define SERVERFRAME_ICONN_H

class IConn {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif //SERVERFRAME_ICONN_H
