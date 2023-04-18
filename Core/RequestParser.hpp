//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_REQUESTPARSER_HPP
#define SERVERFRAME_REQUESTPARSER_HPP

#include <tuple>
#include <iostream>
#include <sstream>
#include <list>
#include <memory>
#include "ContentParser.hpp"
//  from AsioLearn author NearXdu
//  https://github.com/NearXdu/AsioLearn/blob/master/src/example_http/request_handler.hpp
struct Request;
class RequestParser {
private:
    std::list<std::shared_ptr<ContentParser>> contentParser;

public:
    RequestParser();
    ~RequestParser();

    void reset();
    /// 解析结果
    enum ResultEnum { good, bad, indeterminate };

    /// 尝试解析头部
    ResultEnum parseHeader(Request& request, std::stringstream & stream);

    /// 尝试解析表单
    ResultEnum parseForm(Request& request, std::stringstream & stream);

private:
    static ResultEnum parseFirstLine(Request& request, std::string& line);
};


#endif //SERVERFRAME_REQUESTPARSER_HPP
