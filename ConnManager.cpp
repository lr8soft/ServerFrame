//
// Created by lrsoft on 2023/4/11.
//

#include "ConnManager.h"
#include "Utils/LogUtil.h"

ConnManager* ConnManager::pInstance = nullptr;

void ConnManager::startConn(std::shared_ptr<IConn> pConn) {
    std::lock_guard<std::mutex> lock(connMutex);

    connList.insert(pConn);
    pConn->start();
}

void ConnManager::stopConn(std::shared_ptr<IConn> pConn) {
    std::lock_guard<std::mutex> lock(connMutex);
    connList.erase(pConn);
    pConn->stop();
}

void ConnManager::stopAllConn() {
    std::lock_guard<std::mutex> lock(connMutex);
    for(auto conn : connList) {
        conn->stop();
    }
    connList.clear();
}
