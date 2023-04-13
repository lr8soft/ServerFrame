//
// Created by lrsoft on 2023/4/13.
//

#include "RequestDispatcher.h"
#include "Request.hpp"
#include "Reply.h"

RequestDispatcher* RequestDispatcher::pInst = nullptr;

void RequestDispatcher::handleRequest(const Request &req, Reply &rep) {
    auto searchIter = handleGroup.equal_range(req.method);
    // 该404了
    if (searchIter.first == searchIter.second) {
        rep = Reply::stockReply(Reply::not_found);
        return;
    }

    // 依次调用
    for (auto iter = searchIter.first; iter != searchIter.second; ++iter) {
        // 如果有一个handler处理了，那么就不再继续
        if (iter->second->handleRequest(req, rep)) {
            return;
        }
    }
}

void RequestDispatcher::addHandler(const std::string method, std::shared_ptr<IRequestHandler> handler) {
    handleGroup.insert(std::make_pair(method, handler));
}
