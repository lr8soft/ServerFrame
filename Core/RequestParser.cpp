//
// Created by lrsoft on 2023/4/12.
//

#include <vector>
#include <sstream>
#include "RequestParser.hpp"
#include "Request.hpp"
#include "../Utils/UrlUtils.h"

#include "MultiFormParser.h"
#include "X3WFormParser.h"


RequestParser::RequestParser() {
    contentParser.push_back(std::make_shared<MultiFormParser>());
    contentParser.push_back(std::make_shared<X3WFormParser>());
}

RequestParser::~RequestParser() {
    contentParser.clear();
}

void RequestParser::reset() {

}


RequestParser::ResultEnum RequestParser::parseFirstLine(Request& request, std::string &line) {
    // 去掉末尾的\r
    line.pop_back();
    // 读俩空格的位置，没有就凉凉
    auto splitLine1Pos = line.find(' ');
    if(splitLine1Pos == std::string::npos) return bad;
    auto splitLine2Pos = line.find(' ', splitLine1Pos + 1);
    if(splitLine2Pos == std::string::npos) return bad;

    // 读取方法与uri
    request.method = line.substr(0, splitLine1Pos);
    request.uri = line.substr(splitLine1Pos + 1, splitLine2Pos - splitLine1Pos - 1);

    // 解析协议版本
    std::string protocol = line.substr(splitLine2Pos + 1);
    auto slashPos = protocol.find('/');
    if(slashPos == std::string::npos) return bad;
    auto dotPos = protocol.find('.', slashPos + 1);
    if(dotPos == std::string::npos) return bad;

    // 解析1.1 2.0还是啥的
    request.httpVersionMajor = std::stoi(protocol.substr(slashPos + 1, 1));
    request.httpVersionMinor = std::stoi(protocol.substr(dotPos + 1, 1));
    //std::cout << request.method << " " << request.uri << " " << protocol << std::endl;
    return good;
}

RequestParser::ResultEnum RequestParser::parseHeader(Request &request, std::stringstream &stream) {
    RequestParser::ResultEnum result = good;

    bool isFirstLine = true;
    std::string line;
    // HTTP REQUEST中使用\r\n间隔，getline后会剩下\r
    while(std::getline(stream, line)) {
        // 第一行格式POST /api/user/login HTTP/1.1
        if(isFirstLine) {
            if(parseFirstLine(request, line) != good) return bad;
            isFirstLine = false;
            continue;
        }

        // header里的最后一行
        if(line == "\r") {
            std::cout << "header last line " << line << std::endl;
            break;
        }
        // 去掉结尾的\r
        line.pop_back();

        // 每行格式都是xxx: yyyy
        auto splitPos = line.find(": ");
        if(splitPos == std::string::npos) return bad;

        std::string key = line.substr(0, splitPos);
        std::string value = line.substr(splitPos + 2);
        //std::cout << "[" + key + "] " << "[" + value + "]" << std::endl;
        request.headerMap.insert(std::make_pair(key, value));
    }

    if(request.method == "POST") {
        auto typeIter = request.headerMap.find("Content-Type");
        auto lengthIter = request.headerMap.find("Content-Length");

        if(lengthIter == request.headerMap.end()) return indeterminate;
        request.contentLength = std::stoi(lengthIter->second);

        // 长度不是0又没有类型就是没传完
        if(request.contentLength > 0 && typeIter == request.headerMap.end()) {
            return indeterminate;
        }else{
            if(typeIter != request.headerMap.end())
                request.contentType = typeIter->second;
        }
        //std::cout << "Content-Type: " << request.contentType << " Content-Length: " <<  request.contentLength << std::endl;
    }

    return result;
}

RequestParser::ResultEnum RequestParser::parseForm(Request &request, std::stringstream &stream) {
    RequestParser::ResultEnum result = good;
    // 尝试解析body
    if(request.method == "POST") {
        for(auto parser : contentParser){
            auto result = parser->parseBody(request, stream);
            if(result == ContentParser::good) {
                break;
            }else if(result == ContentParser::indeterminate) {
                return indeterminate;
            }
        }
    }
    return result;
}

