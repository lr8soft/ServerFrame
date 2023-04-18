//
// Created by lrsoft on 2023/4/13.
//

#ifndef SERVERFRAME_POSTRESOLVER_H
#define SERVERFRAME_POSTRESOLVER_H
#include "IRequestSolver.h"

class PostResolver: public IRequestSolver{
public:
    /// 处理POST请求
    bool handleRequest(const Request& req, Reply& rep) override;
private:

};


#endif //SERVERFRAME_POSTRESOLVER_H
