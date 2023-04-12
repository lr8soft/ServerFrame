//
// Created by lrsoft on 2023/4/12.
//

#include <fstream>
#include "RequestHandler.h"
#include "Request.hpp"
#include "Reply.h"
#include "../Utils/MimeUtils.h"

RequestHandler::RequestHandler(const std::string &doc_root) : _root(doc_root){

}

void RequestHandler::handleRequest(const Request &req, Reply &rep) {
    std::string reqPath;
    // 解析失败
    if (!urlDecode(req.uri, reqPath))
    {
        rep = Reply::stockReply(Reply::bad_request);
        return;
    }

    // 请求路径不含..
    if (reqPath.empty() || reqPath[0] != '/' || reqPath.find("..") != std::string::npos) {
        rep = Reply::stockReply(Reply::bad_request);
        return;
    }

    // 如果请求url最后一个字符是/，那么加上一个index.html
    if (reqPath[reqPath.size() - 1] == '/') {
        reqPath += "index.html";
    }

    // 计算文件拓展名
    std::size_t last_slash_pos = reqPath.find_last_of("/");
    std::size_t last_dot_pos = reqPath.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
        extension = reqPath.substr(last_dot_pos + 1);
    }

    // 读取目标文件
    std::string full_path = _root + reqPath;//文件的完整目录
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    // 404
    if (!is) {
        rep = Reply::stockReply(Reply::not_found);
        return;
    }

    rep.status = Reply::ok;
    char buf[512];
    while (is.read(buf, sizeof(buf)).gcount() > 0) {
        //append(const char *,size_t size)
        rep.content.append(buf, is.gcount());
    }

    // 拼接响应头
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = MimeUtils::getMimeFromExt(extension);//扩展名->Content-Type

}

bool RequestHandler::urlDecode(const std::string &in, std::string &out) {
    return true;
}
