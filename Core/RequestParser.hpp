//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_REQUESTPARSER_HPP
#define SERVERFRAME_REQUESTPARSER_HPP

#include <tuple>
#include <iostream>
#include <sstream>
//  from AsioLearn author NearXdu
//  https://github.com/NearXdu/AsioLearn/blob/master/src/example_http/request_handler.hpp
struct Request;
class RequestParser {
public:
    RequestParser();

    void reset();
    // 解析结果
    enum ResultEnum { good, bad, indeterminate };

    // 尝试解析
    template <typename T>
    std::tuple<ResultEnum, T> parse(Request& request, T begin, T end) {
        while (begin != end) {
            ResultEnum result = parseRequestItem(request, *begin++);
            if (result == good || result == bad) {
                return std::make_tuple(result, begin);
            }
        }
        return std::make_tuple(indeterminate, begin);
    }

    // 尝试解析表单
    void parseForm(Request& request, std::stringstream & str);

private:
    ResultEnum parseRequestItem(Request& req, char input);


    /// Check if a byte is an HTTP character.
    static bool isChar(int c);

    /// Check if a byte is an HTTP control character.
    static bool isCtl(int c);

    /// Check if a byte is defined as an HTTP tspecial character.
    static bool isTspecial(int c);

    /// Check if a byte is a digit.
    static bool isDigit(int c);

    //解析http请求状态
    enum state
    {
        method_start,                //方法开始
        method,                      //请求方法
        uri,                        //请求uri
        http_version_h,             //h
        http_version_t_1,           //t
        http_version_t_2,           //t
        http_version_p,             //p
        http_version_slash,         //斜杠
        http_version_major_start,   //
        http_version_major,         //主版本号
        http_version_minor_start,
        http_version_minor,         //次版本号
        expecting_newline_1,        //\r\n
        ////////////////////////////////
        header_line_start,//请求头
        header_lws,//
        header_name,//首部key
        space_before_header_value,//之间的空行
        header_value,//首部value
        expecting_newline_2,
        expecting_newline_3
    } _state;
};


#endif //SERVERFRAME_REQUESTPARSER_HPP
