//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_REQUEST_H
#define SERVERFRAME_REQUEST_H

#include <string>
#include <vector>
#include "Header.h"

struct Request {
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<Header> headers;
};

#endif //SERVERFRAME_REQUEST_H
