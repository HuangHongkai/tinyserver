//
// Created by hhk on 18-12-13.
//

#ifndef STRUTILS_H
#define STRUTILS_H

#include <bits/stdc++.h>

using namespace std;

namespace WebServer {
    using std::pair;

    /**
     * @brief 清除空格
     * @param blank
     */
    string& trim_left(string &str);

    string& trim_right(string &str);

    string& trim(string &str);

    string &replace_str(string &str, const string& old_str, const string& new_str, size_t limit = 0x7fffffff);

    string &replace_str(const string &str, const string& old_str, const string& new_str, size_t limit = 0x7fffffff);

    /**
     * @brief 10进制数字转为string
     * @param i
     * @param base ios_base::oct ios_base::hex ios_base::dec
     * @return
     */
    string itos(const long i, const ios_base::fmtflags base = ios_base::dec);

    std::vector<std::string> split(const std::string& input, const std::string& delim);

    pair<string, string> split2(const string& data, const string& delim);

    string& str_tolower(string& str);

    string& str_toupper(string& str);

    string str_format(const char *format, ...);

    template<typename T>
    T str_parse(const string &str) {
        T ans;
        stringstream ss(str);
        ss >> ans;
        return ans;
    }

    /**
     * 检查字符串是不是数字
     * @param str
     * @return
     */
    bool str_isnumber(const string& str);
}
#endif //STRUTILS_H
