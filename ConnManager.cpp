//
// Created by lrsoft on 2023/4/11.
//

#include "ConnManager.h"

ConnManager* ConnManager::pInstance = nullptr;

void ConnManager::addConn(std::shared_ptr<AsioSocket> pConn) {
    connList.push_back(pConn);
}
