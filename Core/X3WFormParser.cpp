//
// Created by lrsoft on 2023/4/15.
//

#include <iostream>
#include "X3WFormParser.h"
#include "Request.hpp"

#include "../Utils/UrlUtils.h"


ContentParser::ResultEnum X3WFormParser::parseBody(Request &request, std::stringstream &stream) {
    ContentParser::ResultEnum result = bad;
    if(request.contentType.find("application/x-www-form-urlencoded") == 0) {
        std::cout << "application/x-www-form-urlencoded" << std::endl;
        // +2因为\r\n不算进x-www-form的长度
        if(!checkBodyComplete(request.contentLength + 2, stream)) {
            std::cout << "x3w-form incompleted" << std::endl;
            return indeterminate;
        }
        result = good;
        // 去掉\r\n
        stream.get();
        stream.get();

        std::string content;
        content.resize(request.contentLength);
        stream.read(&content[0], request.contentLength);

        // 解析-xform，格式username=123456&password=aaaaa
        std::string key, value;
        bool isKey = true;
        for (auto &c : content) {
            if (c == '=') {
                isKey = false;
            } else if (c == '&') {
                auto decodeKey = UrlUtils::urlDecode(key);
                auto decodeValue = UrlUtils::urlDecode(value);
                request.bodyMap[decodeKey] = decodeValue;
                key.clear();
                value.clear();
                isKey = true;
            } else {
                if (isKey) {
                    key.push_back(c);
                } else {
                    value.push_back(c);
                }
            }
        }
        if (!key.empty()) {
            auto decodeKey = UrlUtils::urlDecode(key);
            auto decodeValue = UrlUtils::urlDecode(value);
            request.bodyMap[decodeKey] = decodeValue;
        }

    }

    return result;
}
