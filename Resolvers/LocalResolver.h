//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_LOCALRESOLVER_H
#define SERVERFRAME_LOCALRESOLVER_H

#include <string>
#include "IRequestSolver.h"

class LocalResolver: public IRequestSolver{
public:
    /// 请求带文件夹
    explicit LocalResolver(const std::string& doc_root);

    /// 处理请求带reply
    bool handleRequest(const Request& req, Reply& rep) override;

private:
    /// 网站文件路径
    std::string _root;
};


#endif //SERVERFRAME_LOCALRESOLVER_H
