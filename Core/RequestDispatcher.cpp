//
// Created by lrsoft on 2023/4/13.
//

#include <iostream>
#include "RequestDispatcher.h"
#include "Request.hpp"
#include "Reply.h"

RequestDispatcher* RequestDispatcher::pInst = nullptr;

void RequestDispatcher::handleRequest(const std::string& appName, const Request &req, Reply &rep) {
    // 根据对应的appName依次调用
    auto range = handleMap.equal_range(appName);
    for (auto iter = range.first; iter != range.second; ++iter) {
        // 如果有一个handler处理了，那么就不再继续
        if ((*iter).second->handleRequest(req, rep)) {
            return;
        }
    }
}

void RequestDispatcher::addHandler(const std::string& appName, std::shared_ptr<IRequestSolver> handler) {
    handleMap.insert(std::make_pair(appName, handler));
}
