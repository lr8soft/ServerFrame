//
// Created by lrsoft on 2023/4/10.
//

#include <iostream>
#include "Application.h"
#include "Utils/LogUtil.h"
#include "Utils/HttpConnect.h"

Application* Application::pInstance = nullptr;

Application *Application::getInstance() {
    if(pInstance == nullptr)
        pInstance =  new Application;
    return nullptr;
}

void Application::init() {
    HttpConnect conn;
    std::cout << conn.readFromUrl("https://raw.githubusercontent.com/lr8soft/MoreElectricTools/master/README.txt");
}

void Application::work() {
    while(!getIsTerminated()){
        timer.Tick();
        LogUtil::printWarn(std::to_string(timer.getAccumlateTime()));
    }
}

void Application::finalize() {

}

bool Application::getIsTerminated() {
    return isTerminated;
}
