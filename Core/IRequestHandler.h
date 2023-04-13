//
// Created by lrsoft on 2023/4/13.
//

#ifndef SERVERFRAME_IREQUESTHANDLER_H
#define SERVERFRAME_IREQUESTHANDLER_H

struct Request;
struct Reply;
class IRequestHandler{
public:
    /// 解析req并返回rep，成功则返回true
    virtual bool handleRequest(const Request& req, Reply& rep) = 0;
};

#endif //SERVERFRAME_IREQUESTHANDLER_H
