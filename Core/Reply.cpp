//
// Created by lrsoft on 2023/4/12.
//

#include <asio.hpp>
#include "Reply.h"

asio::const_buffer toBuffer(Reply::StatusType status) {
    switch (status) {
        case Reply::ok:
            return asio::buffer(StatusString::ok);
        case Reply::created:
            return asio::buffer(StatusString::created);
        case Reply::accepted:
            return asio::buffer(StatusString::accepted);
        case Reply::no_content:
            return asio::buffer(StatusString::no_content);
        case Reply::multiple_choices:
            return asio::buffer(StatusString::multiple_choices);
        case Reply::moved_permanently:
            return asio::buffer(StatusString::moved_permanently);
        case Reply::moved_temporarily:
            return asio::buffer(StatusString::moved_temporarily);
        case Reply::not_modified:
            return asio::buffer(StatusString::not_modified);
        case Reply::bad_request:
            return asio::buffer(StatusString::bad_request);
        case Reply::unauthorized:
            return asio::buffer(StatusString::unauthorized);
        case Reply::forbidden:
            return asio::buffer(StatusString::forbidden);
        case Reply::not_found:
            return asio::buffer(StatusString::not_found);
        case Reply::internal_server_error:
            return asio::buffer(StatusString::internal_server_error);
        case Reply::not_implemented:
            return asio::buffer(StatusString::not_implemented);
        case Reply::bad_gateway:
            return asio::buffer(StatusString::bad_gateway);
        case Reply::service_unavailable:
            return asio::buffer(StatusString::service_unavailable);
        default:
            return asio::buffer(StatusString::internal_server_error);
    }
}

std::vector<asio::const_buffer> Reply::toBuffers() {
    std::vector<asio::const_buffer> buffers;
    // 创建响应行
    buffers.push_back(toBuffer(status));
    for (std::size_t i = 0; i < headers.size(); ++i)
    {
        Header& h = headers[i];
        // 响应key
        buffers.push_back(asio::buffer(h.name));
        buffers.push_back(asio::buffer(misc_strings::name_value_separator));
        // 响应value
        buffers.push_back(asio::buffer(h.value));
        buffers.push_back(asio::buffer(misc_strings::crlf));
    }
    //  回车换行
    buffers.push_back(asio::buffer(misc_strings::crlf));
    //  响应报文
    buffers.push_back(asio::buffer(content));
    return buffers;
}

Reply Reply::stockReply(Reply::StatusType status) {
    Reply rep;
    rep.status = status;
    rep.content = stock_replies::toString(status);
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = "text/html";
    return rep;
}
