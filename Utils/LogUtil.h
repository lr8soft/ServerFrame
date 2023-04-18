//
// Created by lrsoft on 2023/4/10.
//

#ifndef SERVERFRAME_LOGUTIL_H
#define SERVERFRAME_LOGUTIL_H

#include <iostream>
#include <string>
#include <mutex>

#ifndef _WIN64
#define LOG_RED      "\033[31m"   /* Red */
#define LOG_YELLOW   "\033[33m"   /* Yellow */
#define LOG_WHITE    "\033[37m"   /* White */

#else
#include <Windows.h>
#endif
class LogUtil {
private:
    static std::mutex logMutex;
public:
    static void printInfo(const std::string& str);

    static void printWarn(const std::string& str);

    static void printError(const std::string& str);

    static std::string getCurrentTime();
};
#endif //SERVERFRAME_LOGUTIL_H
