//
// Created by lrsoft on 2023/4/13.
//

#ifndef SERVERFRAME_URLUTILS_H
#define SERVERFRAME_URLUTILS_H
#include <string>

namespace UrlUtils {
    // 把url中转义字符空格等转换回来
    bool urlDecode(const std::string& in, std::string& out);

} // namespace url-utils


#endif //SERVERFRAME_URLUTILS_H
