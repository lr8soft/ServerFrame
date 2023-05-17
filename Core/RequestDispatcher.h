//
// Created by lrsoft on 2023/4/13.
//

#ifndef SERVERFRAME_REQUESTDISPATCHER_H
#define SERVERFRAME_REQUESTDISPATCHER_H
#include <map>
#include <memory>
#include <list>
#include <string>
#include "../Resolvers/IRequestSolver.h"

class RequestDispatcher {
public:
    static RequestDispatcher* getInstance(){
        if(pInst == nullptr)
            pInst = new RequestDispatcher;
        return pInst;
    }

    void addHandler(const std::string& appName, std::shared_ptr<IRequestSolver> handler);
    void handleRequest(const std::string& appName, const Request& req, Reply& rep);
private:
    static RequestDispatcher* pInst;

    // Requesthandler list
    std::multimap<std::string, std::shared_ptr<IRequestSolver>> handleMap;
};


#endif //SERVERFRAME_REQUESTDISPATCHER_H
