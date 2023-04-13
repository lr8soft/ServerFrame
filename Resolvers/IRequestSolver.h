//
// Created by lrsoft on 2023/4/13.
//

#ifndef SERVERFRAME_IREQUESTSOLVER_H
#define SERVERFRAME_IREQUESTSOLVER_H

struct Request;
struct Reply;
class IRequestSolver{
public:
    /// 解析req并返回rep，成功则返回true
    virtual bool handleRequest(const Request& req, Reply& rep) = 0;
};

#endif //SERVERFRAME_IREQUESTSOLVER_H
