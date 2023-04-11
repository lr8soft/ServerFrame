//
// Created by lrsoft on 2023/4/11.
//

#ifndef SERVERFRAME_CONNMANAGER_H
#define SERVERFRAME_CONNMANAGER_H

#include "SimpleAsioDefine.h"
#include <map>

class ConnManager {
public:
    static ConnManager* getInstance(){
        if(pInstance == nullptr)
            pInstance = new ConnManager;
        return pInstance;
    }
private:
    ConnManager(){}

    static ConnManager* pInstance;
};


#endif //SERVERFRAME_CONNMANAGER_H
