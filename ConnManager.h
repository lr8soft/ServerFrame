//
// Created by lrsoft on 2023/4/11.
//

#ifndef SERVERFRAME_CONNMANAGER_H
#define SERVERFRAME_CONNMANAGER_H

#include "SimpleAsioDefine.h"
#include <vector>
#include <memory>

class ConnManager {
public:
    static ConnManager* getInstance(){
        if(pInstance == nullptr)
            pInstance = new ConnManager;
        return pInstance;
    }

    void addConn(std::shared_ptr<AsioSocket> pConn);
private:
    ConnManager(){}

private:
    std::vector<std::shared_ptr<AsioSocket>> connList;

    static ConnManager* pInstance;
};


#endif //SERVERFRAME_CONNMANAGER_H
