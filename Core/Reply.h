//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_REPLY_H
#define SERVERFRAME_REPLY_H

#include <string>
#include <vector>

#include "Header.h"
#include "../SimpleAsioDefine.h"

struct Reply {
    enum StatusType {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        multiple_choices = 300,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503
    } status;
    std::vector<Header> headers;

    /// 回复内容
    std::string content;

    /// 转buff
    std::vector<asio::const_buffer> toBuffers();

    static Reply stockReply(StatusType status);
    static void setReply(Reply& reply, const std::string& content, const std::string& extName, StatusType status = ok);
};




#endif //SERVERFRAME_REPLY_H
