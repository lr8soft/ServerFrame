//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_REQUESTHANDLER_H
#define SERVERFRAME_REQUESTHANDLER_H

#include <string>
#include "IRequestHandler.h"

class RequestHandler: public IRequestHandler{
public:
    //禁止拷贝构造函数和赋值重载
    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    /// 请求带文件夹
    explicit RequestHandler(const std::string& doc_root);

    /// 处理请求带reply
    bool handleRequest(const Request& req, Reply& rep) override;

private:
    /// 网站文件路径
    std::string _root;
};


#endif //SERVERFRAME_REQUESTHANDLER_H
