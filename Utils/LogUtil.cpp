//
// Created by lrsoft on 2023/4/10.
//

#include "LogUtil.h"

std::mutex LogUtil::logMutex;

void LogUtil::printInfo(const std::string &str) {
    std::thread::id tid = std::this_thread::get_id();
    std::lock_guard<std::mutex> guard(logMutex);
#ifndef _WIN64
    std::cout << LOG_WHITE << "[INFO][Thread-" << tid << "]" << str << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::cout << "[INFO][Thread-" << tid << "]" << str << std::endl;
#endif
}

void LogUtil::printWarn(const std::string &str) {
    std::thread::id tid = std::this_thread::get_id();
    std::lock_guard<std::mutex> guard(logMutex);
#ifndef _WIN64
    std::cout << LOG_YELLOW << "[WARN][Thread-" << tid << "]" << str << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

    std::cout << "[WARN][Thread-" << tid << "]" << str << std::endl;
#endif
}

void LogUtil::printError(const std::string &str) {
    std::thread::id tid = std::this_thread::get_id();
    std::lock_guard<std::mutex> guard(logMutex);
#ifndef _WIN64
    std::cout << LOG_RED << "[ERROR][Thread-" << tid << "]" << str << std::endl;
#else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    std::cout << "[ERROR][Thread-" << tid << "]" << str << std::endl;
#endif
}
