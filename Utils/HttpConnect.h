//
// Created by lrsoft on 2023/4/7.
//

#ifndef SERVERFRAME_HTTPCONNECT_H
#define SERVERFRAME_HTTPCONNECT_H

#include <iostream>
class HttpConnect {
private:

    static size_t writeData(void * buffer, size_t size, size_t membercount, void * userp);
public:
    static bool haveGlobalInit;
    static void releaseGlobalEnov();

    std::string readFromUrl(const std::string url);

    HttpConnect();


};

#endif //SERVERFRAME_HTTPCONNECT_H
