//
// Created by lrsoft on 2023/4/15.
//

#ifndef SERVERFRAME_X3WFORMPARSER_H
#define SERVERFRAME_X3WFORMPARSER_H
#include "ContentParser.hpp"
class X3WFormParser: public ContentParser {
public:
    ResultEnum parseBody(Request &request, std::stringstream &stream) override;
};


#endif //SERVERFRAME_X3WFORMPARSER_H
