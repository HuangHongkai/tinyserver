//
// Created by hhk on 18-12-13.
//

#ifndef SIMPLE_JSON_STRUTILS_H
#define SIMPLE_JSON_STRUTILS_H

#include <bits/stdc++.h>

namespace EasyJson {
    using std::string;
    using std::ios_base;
    using std::ptr_fun;

    /**
     * @brief 清除空格
     * @param blank
     */
    string& trim_left(string &str);

    string& trim_right(string &str);

    string& trim(string &str);

    string &replace_str(string &str, const string& old_str, const string& new_str, size_t limit = 0x7fffffff);

    string &replace_str(const string &str, const string& old_str, const string& new_str, size_t limit = 0x7fffffff);

}
#endif //SIMPLE_JSON_STRUTILS_H
