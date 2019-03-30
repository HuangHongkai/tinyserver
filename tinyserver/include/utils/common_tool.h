//
// Created by hhk on 18-12-19.
//

#include <cstdio>
#include <bits/stdc++.h>
#include "JSONObject.h"
#include <http/HttpResponse.h>

/**
 * 通用工具箱
 */
namespace WebServer{
    using std::string;

    long get_file_size(FILE* f);

    long get_file_size(const string& path);

}