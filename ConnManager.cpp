//
// Created by lrsoft on 2023/4/11.
//

#include "ConnManager.h"
#include "Utils/LogUtil.h"

ConnManager* ConnManager::pInstance = nullptr;

void ConnManager::startConn(std::shared_ptr<Connection> pConn) {
    connList.insert(pConn);
    pConn->start();
}

void ConnManager::stopConn(std::shared_ptr<Connection> pConn) {
    connList.erase(pConn);
    pConn->stop();
}

void ConnManager::stopAllConn() {
    for(auto conn : connList) {
        conn->stop();
    }
    connList.clear();
}
