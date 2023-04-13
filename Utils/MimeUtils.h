//
// Created by lrsoft on 2023/4/12.
//

#ifndef SERVERFRAME_MIMEUTILS_H
#define SERVERFRAME_MIMEUTILS_H
#include <string>

namespace MimeUtils {
    /// 转换拓展名到mime类型
    std::string getMimeFromExt(const std::string& extension);

} // namespace mime_utils


#endif //SERVERFRAME_MIMEUTILS_H
