//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_REQUEST_H
#define SERVERFRAME_REQUEST_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "Header.h"

struct Attachment{
    std::stringstream content;
    std::string filename;
    std::string contentType;
};

struct Request {
    std::string method;
    std::string uri;
    std::string contentType;
    int contentLength;
    int httpVersionMajor;
    int httpVersionMinor;
    std::map<std::string, std::string> headerMap;
    std::map<std::string, std::string> bodyMap;
    std::map<std::string, Attachment> fileMap;

    void reset() {
        method.clear();
        uri.clear();
        contentType.clear();
        headerMap.clear();
        bodyMap.clear();
        fileMap.clear();
    }
};

#endif //SERVERFRAME_REQUEST_H
