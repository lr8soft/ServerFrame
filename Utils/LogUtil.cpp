//
// Created by lrsoft on 2023/4/10.
//

#include <iomanip>
#include <chrono>
#include "LogUtil.h"

std::mutex LogUtil::logMutex;

void LogUtil::printInfo(const std::string &str) {
    std::lock_guard<std::mutex> guard(logMutex);
#ifndef _WIN64
    std::cout << LOG_WHITE << "[" << getCurrentTime() << "]"<< str << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::cout << "[" << getCurrentTime() << "]" << str << std::endl;
#endif
}

void LogUtil::printWarn(const std::string &str) {
    std::lock_guard<std::mutex> guard(logMutex);
#ifndef _WIN64
    std::cout << LOG_YELLOW << "[" << getCurrentTime() << "]" << str << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

    std::cout << "[" <<getCurrentTime() << "]"<< str << std::endl;
#endif
}

void LogUtil::printError(const std::string &str) {
    std::lock_guard<std::mutex> guard(logMutex);
#ifndef _WIN64
    std::cout << LOG_RED << "[" << getCurrentTime() << "]"  << str << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    std::cout << "[" << getCurrentTime() << "]"  << str << std::endl;
#endif
}

std::string LogUtil::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", std::localtime(&now_c));
    return std::string(buf);
}