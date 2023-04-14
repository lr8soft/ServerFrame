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

    std::cout << "post content:" << std::endl;
    // 遍历req.contentMap并输出
    for (auto &item : req.contentMap) {
        std::cout << item.first << ":" << item.second << " len: " << item.second.length() << std::endl;
    }

    return true;
}
