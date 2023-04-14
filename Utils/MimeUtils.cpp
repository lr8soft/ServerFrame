//
// Created by lrsoft on 2023/4/12.
//

#include "MimeUtils.h"

namespace MimeUtils{
    struct mapping {
        const char *extension;
        const char *mime_type;
    } mappings[] = {
            {"gif",  "image/gif"},
            {"htm",  "text/html"},
            {"html", "text/html"},
            {"jpg",  "image/jpeg"},
            {"png",  "image/png"},
            {"css",  "text/css"},
            {"js",   "application/javascript"},
            {"json", "application/json"}
    };

    std::string getMimeFromExt(const std::string& extension) {
        for (mapping m: mappings) {
            if (m.extension == extension) {
                return m.mime_type;
            }
        }
        return "text/plain";
    }
}