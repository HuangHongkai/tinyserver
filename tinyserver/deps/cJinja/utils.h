//
// Created by Administrator on 2019/3/30 0030.
//

#ifndef CJINJA_UTILS_H
#define CJINJA_UTILS_H
#include <ctype.h>
#include <bits/stdc++.h>

namespace cJinja {
    using std::string;
    using std::pair;
    using std::stringstream;

    string str_format(const char *format, ...) {
        size_t len = strlen(format) + 200;
        char *szBuffer = new char[len];
        memset(szBuffer, 0x00, len);
        va_list ap;
        va_start(ap, format);
        vsnprintf(szBuffer, len, format, ap);
        string s(szBuffer);
        delete []szBuffer;
        return s;
    }

    long get_file_size(FILE *f) {
        long size = -1;
        long cur = ftell(f);
        if (f != nullptr) {
            fseek (f, 0, SEEK_END);
            size = ftell (f);
            fseek (f, cur, SEEK_SET);
        }
        return size;
    }

    bool str_isnumber(const string &str) {
        bool is_float = false;
        for(auto& ch: str) {
            if(!isdigit(ch)) {
                if(ch == '.' && !is_float)
                    is_float = true;
                else
                    return false;
            }
        }
        return true;
    }

    pair<string, string> split2(const string &data, const string &delim) {
        size_t pos = data.find(delim);
        if(pos != string::npos) {
            return {data.substr(0, pos), data.substr(pos+delim.size())};
        }
        return {data, ""};
    }

    template<typename T>
    T str_parse(const string &str) {
        T ans;
        stringstream ss(str);
        ss >> ans;
        return ans;
    }
}

#endif //CJINJA_UTILS_H
