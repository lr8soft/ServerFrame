//
// Created by lrsoft on 2023/4/13.
//

#include <iostream>
#include "PostResolver.h"
#include "../Core/Request.hpp"
#include "../Core/Reply.h"
#include "../Utils/UrlUtils.h"

bool PostResolver::handleRequest(const Request &req, Reply &rep) {
    std::string reqPath;
    // 解析失败
    if (!UrlUtils::urlDecode(req.uri, reqPath)) {
        rep = Reply::stockReply(Reply::bad_request);
        return false;
    }

    // 打印req.headers里的内容
    for (auto &header : req.headers) {
        std::cout << header.name << ": " << header.value << std::endl;
    }

    //req.

    return false;
}
