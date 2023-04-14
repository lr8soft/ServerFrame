//
// Created by lrsoft on 2023/4/13.
//

#include <iostream>
#include <sstream>
#include "PostResolver.h"
#include "../Core/Request.hpp"
#include "../Core/Reply.h"
#include "../Utils/UrlUtils.h"
#include "../Utils/MimeUtils.h"

bool PostResolver::handleRequest(const Request &req, Reply &rep) {
    std::string reqPath;
    // 解析失败
    if (!UrlUtils::urlDecode(req.uri, reqPath)) {
        rep = Reply::stockReply(Reply::bad_request);
        return false;
    }

    std::stringstream stream;
    stream << "POST Body Params:\n";
    // 遍历req.contentMap并输出
    for (auto &item : req.bodyMap) {
        stream << item.first << ":" << item.second << " len: " << item.second.length() << std::endl;
    }

    Reply::setReply(rep, stream.str(), "html");
    return true;
}
