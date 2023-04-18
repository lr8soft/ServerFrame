//
// Created by lrsoft on 2023/4/15.
//

#include <iostream>
#include "Request.hpp"
#include "MultiFormParser.h"
#include "../Utils/UrlUtils.h"

ContentParser::ResultEnum MultiFormParser::parseBody(Request &request, std::stringstream &stream) {
    ContentParser::ResultEnum result = bad;
    if(request.contentType.starts_with("multipart/form-data")) {
        //std::cout << "multipart/form-data" << std::endl;
        // multipart长度包含\r\n的
        if(!checkBodyComplete(request.contentLength - 2, stream)) {
            //std::cout << "form-data incompleted" << std::endl;
            return indeterminate;
        }
        // 读取boundary
        std::string boundary;
        auto pos = request.contentType.find("boundary=");
        if(pos != std::string::npos) {
            boundary = "--" +  request.contentType.substr(pos + 9);
        }
        if(boundary.empty()) {
            std::cout << "no boundary " << request.contentType << std::endl;
            return bad;
        }
        result = good;

        // 读取body
        std::string body;
        body.resize(request.contentLength);
        stream.read(&body[0], request.contentLength);

        // 将body按行分割，boundary作为数据分割线，包围中间的数据
        // 其中数据名称name在数据里的第一行，以name=""结束
        // 第二行，以\r\n结束
        // 第三行是数据值，再将name与value作为kv值记录到bodyMap
        std::string key, value;
        bool isValue = false;
        std::string line;
        std::stringstream bodyStream(body);
        while (std::getline(bodyStream, line)) {
            //std::cout << line << std::endl;
            // 末尾除了最后一行，去掉getline后剩下的\r
            line.pop_back();
            //std::cout << "boundary [" + line +"]" <<"[" + boundary + "]" << std::endl;
            if(line == boundary) {

                // 读到分割线，说明上一个数据读完了
                if(!key.empty()) {
                    auto decodeKey = UrlUtils::urlDecode(key);
                    auto decodeValue = UrlUtils::urlDecode(value);
                    request.bodyMap[decodeKey] = decodeValue;
                    //std::cout << "key:" << decodeKey << " value:" << decodeValue << std::endl;
                    key.clear();
                    value.clear();
                    isValue = false;
                }
            }else if(line == boundary + "--") {
                // 读到结束分割线，说明数据读完了
                // pop掉了末尾，所以是-不是--
                if(!key.empty()) {
                    auto decodeKey = UrlUtils::urlDecode(key);
                    auto decodeValue = UrlUtils::urlDecode(value);
                    request.bodyMap[decodeKey] = decodeValue;
                    //std::cout << "key:" << decodeKey << " value:" << decodeValue << std::endl;
                    key.clear();
                    value.clear();
                    isValue = false;
                }
                break;
            }else if(line.find("Content-Disposition: form-data; name=") == 0) {
                // 读到数据名称
                if(line.find("filename=") == std::string::npos) {
                    auto pos = line.find("name=\"");
                    if(pos != std::string::npos) {
                        key = line.substr(pos + 6);
                        // 弹掉引号
                        key.pop_back();
                    }
                }else{
                    // 读到文件域，记录name
                    auto pos = line.find("name=\"");
                    if(pos != std::string::npos) {
                        // 把原始字符串记录下来name="xxx"; filename="yyyy"
                        key = line.substr(pos);
                    }
                }
            }else if(line.starts_with("Content-Type: ")){
                continue;
            }else if(line.empty()) {
                // 读到空行，说明数据值开始了
                isValue = true;
            }else if(isValue) {
                // 读到数据值
                value += line;
            }
        }
    }

    return result;
}
