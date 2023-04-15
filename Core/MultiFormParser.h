//
// Created by lrsoft on 2023/4/15.
//

#ifndef SERVERFRAME_MULTIFORMPARSER_H
#define SERVERFRAME_MULTIFORMPARSER_H
#include "ContentParser.hpp"
class MultiFormParser: public ContentParser{
public:
    ResultEnum parseBody(Request &request, std::stringstream &stream) override;
};


#endif //SERVERFRAME_MULTIFORMPARSER_H
