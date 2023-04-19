//
// Created by lrsoft on 2023/4/13.
//

#include <iostream>
#include "RequestDispatcher.h"
#include "Request.hpp"
#include "Reply.h"

RequestDispatcher* RequestDispatcher::pInst = nullptr;

void RequestDispatcher::handleRequest(const Request &req, Reply &rep) {
    // 依次调用
    for (auto iter = handleList.begin(); iter != handleList.end(); ++iter) {
        // 如果有一个handler处理了，那么就不再继续
        if ((*iter)->handleRequest(req, rep)) {
            return;
        }
    }
}

void RequestDispatcher::addHandler( std::shared_ptr<IRequestSolver> handler) {
    handleList.push_back(handler);
}
