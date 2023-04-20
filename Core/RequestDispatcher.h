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

    void addHandler(std::shared_ptr<IRequestSolver> handler);
    void handleRequest(const Request& req, Reply& rep);
private:
    static RequestDispatcher* pInst;

    // Requesthandler list
    std::list<std::shared_ptr<IRequestSolver>> handleList;
};


#endif //SERVERFRAME_REQUESTDISPATCHER_H
