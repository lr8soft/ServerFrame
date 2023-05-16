//
// Created by lrsof on 2023/5/16.
//

#ifndef SERVERFRAME_PATHUTILS_H
#define SERVERFRAME_PATHUTILS_H

#include <string>
class PathUtils {
public:
    // debug与release读取路径不一致
    // 只能转换file/a.text类型的路径，开头不带斜杠
    static std::string getRealPath(const std::string & path);
};


#endif //SERVERFRAME_PATHUTILS_H
