//
// Created by hhk on 18-12-19.
//

#include "utils/common_tool.h"

long WebServer::get_file_size(FILE *f) {
    long size = -1;
    long cur = ftell(f);
    if (f != nullptr) {
        fseek (f, 0, SEEK_END);
        size = ftell (f);
        fseek (f, cur, SEEK_SET);
    }
    return size;
}

long WebServer::get_file_size(const std::string &path) {
    auto f = fopen(path.c_str(), "rb");
    auto size = get_file_size(f);
    fclose(f);
    return size;
}
