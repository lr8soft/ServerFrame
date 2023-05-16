//
// Created by lrsoft on 2023/5/16.
//

#include <sstream>
#include "PathUtils.h"

std::string PathUtils::getRealPath(const std::string & path) {
    std::stringstream ss;
#ifdef _DEBUG
    ss << "../";
#endif
    ss << path;
    return ss.str();
}
