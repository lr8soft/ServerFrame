//
// Created by lrsoft on 2023/4/15.
//

#include <iostream>
#include <iomanip>
#include "Request.hpp"
#include "MultiFormParser.h"
#include "../Utils/UrlUtils.h"

ContentParser::ResultEnum MultiFormParser::parseBody(Request &request, std::stringstream &stream) {
    ContentParser::ResultEnum result = bad;
    if(request.contentType.starts_with("multipart/form-data")) {
        //std::cout << "multipart/form-data " << request.contentLength - 2 << std::endl;
        //std::cout << stream.tellg() << " " << stream.tellp() << std::endl;
        // multipart长度包含\r\n的
        if(!checkBodyComplete(request.contentLength - 2, stream)) {
            //std::cout << "form-data incompleted" << std::endl;
            return indeterminate;
        }
        // 读取boundary
        std::string boundary;
        auto pos = request.contentType.find("boundary=");
        if(pos != std::string::npos) {
            boundary = "--" +  request.contentType.substr(pos + 9);
        }
        if(boundary.empty()) {
            std::cout << "no boundary " << request.contentType << std::endl;
            return bad;
        }
        result = good;

        // 读取body
        std::string body;
        body.resize(request.contentLength);
        stream.read(&body[0], request.contentLength);

        // 将body按行分割，boundary作为数据分割线，包围中间的数据
        // 其中数据名称name在数据里的第一行，以name=""结束
        // 第二行，以\r\n结束
        // 第三行是数据值，再将name与value作为kv值记录到bodyMap
        std::string key, value;
        bool isValue = false;
        std::string line;
        std::stringstream bodyStream(body);

        // 上传的附件
        bool isAttachmentData = false;
        std::string fileType, fileName, fileKeyName;
        std::fpos<mbstate_t> attachmentDataStartPos;

        std::string boundaryEnd = boundary + "--";
        while (std::getline(bodyStream, line)) {
            //std::cout << line << std::endl;
            // 末尾除了最后一行，去掉getline后剩下的\r
            line.pop_back();
            //std::cout << "boundary [" + line +"]" <<"[" + boundary + "]" << std::endl;
            if(line == boundary) {

                // 读到分割线，说明上一个数据读完了
                if(!key.empty()) {
                    auto decodeKey = UrlUtils::urlDecode(key);
                    auto decodeValue = UrlUtils::urlDecode(value);
                    request.bodyMap[decodeKey] = decodeValue;
                    //std::cout << "key:" << decodeKey << " value:" << decodeValue << std::endl;
                    key.clear();
                    value.clear();
                    isValue = false;
                }
            }else if(line == boundaryEnd) {
                // 读到结束分割线，说明数据读完了
                // pop掉了末尾，所以是-不是--
                //std::cout << "last line" << std::endl;
                if(!key.empty()) {
                    auto decodeKey = UrlUtils::urlDecode(key);
                    auto decodeValue = UrlUtils::urlDecode(value);
                    request.bodyMap[decodeKey] = decodeValue;
                    //std::cout << "key:" << decodeKey << " value:" << decodeValue << std::endl;
                    key.clear();
                    value.clear();
                    isValue = false;
                }

                if(isAttachmentData) {
                    // 是附件，从bodyStream读取指定长度二进制数据
                    auto binaryDataEndPos = bodyStream.tellg();
                    std::fpos<mbstate_t> binaryDataLength = binaryDataEndPos - attachmentDataStartPos - boundaryEnd.length() - 2;
                    std::cout << "attachment Length:" << binaryDataLength << std::endl;

                    Attachment attachment;
                    attachment.contentType = fileType;
                    attachment.filename = fileName;

                    std::string buffer;
                    bodyStream >> std::setw(binaryDataLength) >> buffer;
                    attachment.content << buffer;


                    request.fileMap[fileKeyName] = std::move(attachment);
                    //std::cout << "fileKeyName " << "[" + fileKeyName + "]" << " fileName " << "[" + fileName + "]" << " contentType" << "[" + fileType + "]" << std::endl;
                }
                break;
            }else if(line.find("Content-Disposition: form-data; name=") == 0) {
                // 读到数据名称
                if(line.find("filename=") == std::string::npos) {
                    auto pos = line.find("name=\"");
                    if(pos != std::string::npos) {
                        key = line.substr(pos + 6);
                        // 弹掉引号
                        key.pop_back();
                    }
                }else{
                    // Content-Disposition: form-data; name="file"; filename="鏃犳爣棰?png"; filename*=UTF-8''%E6%97%A0%E6%A0%87%E9%A2%98.png
                    // 其中每个项目都已key="value";形式存在，以; 分割
                    // 读取name开头的pos
                    auto pos = line.find("name=\"");
                    // 读取name结尾的";"
                    auto pos2 = line.find("\"", pos + 6);
                    // 读取filename开头的pos
                    auto pos3 = line.find("filename=\"");
                    // 读取filename结尾的"\""
                    auto pos4 = line.find("\"", pos3 + 10);

                    fileKeyName = line.substr(pos + 6, pos2 - pos - 6);
                    fileName = line.substr(pos3 + 10, pos4 - pos3 - 10);

                    //std::cout << "fileKeyName " << "[" + fileKeyName + "]" << " fileName " << "[" + fileName + "]" << std::endl;
                }
            }else if(line.starts_with("Content-Type: ")){
                // 记录附件类型
                fileType = line.substr(14);

                isAttachmentData = true;
                // 记录二进制数据开始位置
                attachmentDataStartPos = bodyStream.tellg();
            }else if(line.empty()) {
                // 读到空行，说明数据值开始了
                isValue = true;
            }else if(isValue) {
                // 读到数据值
                value += line;
            }
        }
    }

    return result;
}
