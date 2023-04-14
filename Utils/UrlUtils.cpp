//
// Created by lrsoft on 2023/4/13.
//

#include <iostream>
#include <sstream>
#include "UrlUtils.h"

namespace UrlUtils {
    bool urlDecode(const std::string &in, std::string &out) {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
            // 转义字符
            if (in[i] == '%') {
                if (i + 3 <= in.size()) {
                    int value = 0;
                    // 跳过%直接取%xy里的xy
                    std::istringstream is(in.substr(i + 1, 2));
                    // 再把xy从16进制转10进制（0-255）
                    if (is >> std::hex >> value) {
                        out += static_cast<char>(value);
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
                // +表示空格
            } else if (in[i] == '+') {
                out += ' ';
            } else {
                out += in[i];
            }
        }
        return true;
    }

    std::string urlDecode(const std::string& in) {
        std::string out;
        urlDecode(in, out);
        return out;
    }
}