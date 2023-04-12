//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_REQUESTHANDLER_H
#define SERVERFRAME_REQUESTHANDLER_H

#include <string>
struct Reply;
struct Request;

class RequestHandler {
public:
    //禁止拷贝构造函数和赋值重载
    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    /// 请求带文件夹
    explicit RequestHandler(const std::string& doc_root);

    /// 处理请求带reply
    void handleRequest(const Request& req, Reply& rep);

private:
    /// 网站文件路径
    std::string _root;

    /// 解析url到out，失败就false
    static bool urlDecode(const std::string& in, std::string& out);//URL分析
};


#endif //SERVERFRAME_REQUESTHANDLER_H
