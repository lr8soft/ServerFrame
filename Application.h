//
// Created by lrsoft on 2023/4/10.
//

#ifndef SERVERFRAME_APPLICATION_H
#define SERVERFRAME_APPLICATION_H

#include <atomic>
#include "Utils/Timer.h"
class Application {
private:
    static Application *pInstance;

    std::atomic<bool> isTerminated = false;

    Timer timer;
public:
    static Application* getInstance();

    void init();
    void work();
    void finalize();

    inline bool getIsTerminated();
};


#endif //SERVERFRAME_APPLICATION_H
