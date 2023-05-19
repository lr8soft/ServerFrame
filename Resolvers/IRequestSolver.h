//
// Created by lrsoft on 2023/4/13.
//

#ifndef SERVERFRAME_IREQUESTSOLVER_H
#define SERVERFRAME_IREQUESTSOLVER_H

struct Request;
struct Reply;
class IRequestSolver{
public:
    /// 解析req并返回rep，成功则返回true
    virtual bool handleRequest(const Request& req, Reply& rep) = 0;

    /// 返回优先级，越大越优先
    virtual int getPriority() = 0;
};

/// 对比器，用于优先队列
class ISolverCompare{
public:
    bool operator()(const std::pair<std::string, std::shared_ptr<IRequestSolver>> & lhs,
            const std::pair<std::string, std::shared_ptr<IRequestSolver>> & rhs) const {
        return lhs.first > rhs.first || (lhs.first == rhs.first && lhs.second->getPriority() > rhs.second->getPriority());
    }

    bool operator()(const std::string & lhs, const std::string & rhs) const{
        return lhs > rhs;
    }

};

#endif //SERVERFRAME_IREQUESTSOLVER_H
