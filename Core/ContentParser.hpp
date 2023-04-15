//
// Created by lrsoft on 2023/4/15.
//

#ifndef SERVERFRAME_CONTENTPARSER_HPP
#define SERVERFRAME_CONTENTPARSER_HPP
#include <sstream>

struct Request;
class ContentParser {
public:
    enum ResultEnum {
        good, bad, indeterminate
    };

    virtual ResultEnum parseBody(Request &request, std::stringstream &stream) = 0;

protected:
    /// 检测请求body长度是否完整
    inline bool checkBodyComplete(int bodyLength, std::stringstream & stream) {
        // tellp是写入指针位置，即当前长度
        // 检测当前读取位置+body长度是否大于当前流长度，就知道body是否完整
        if(bodyLength + stream.tellg() > stream.tellp()) {
            return false;
        }
        return true;
    }
};




#endif //SERVERFRAME_CONTENTPARSER_HPP
