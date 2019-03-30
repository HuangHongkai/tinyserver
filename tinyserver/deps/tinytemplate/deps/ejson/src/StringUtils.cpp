//
// Created by hhk on 18-12-13.
//

#include "StringUtils.h"

namespace EasyJson {
    string& trim_left(string &str)  {
        string::iterator p = find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace)));
        str.erase(str.begin(), p);
        return str;
    }

    string& trim_right(string &str)  {
        string::reverse_iterator p = find_if(str.rbegin(), str.rend(), not1(ptr_fun<int , int>(isspace)));
        str.erase(p.base(), str.end());
        return str;
    }

    string& trim(string &str) {
        return trim_left(trim_right(str));
    }

    string &replace_str(const string &str, const string& old_str, const string& new_str, size_t limit) {
        return replace_str(const_cast<string&>(str), old_str, new_str, limit);
    }

    string &replace_str(string &str, const string& old_str, const string& new_str, size_t limit) {
        size_t pos = 0;
        if(old_str.empty() || limit == 0) return str;
        size_t diff = new_str.length();
        size_t cnt = 0;
        while ((pos=str.find(old_str, pos))!= string::npos ) {
            str.replace( pos, old_str.length(), new_str );
            pos += diff;
            if(++cnt>=limit)
                break;
        }
        return str;
    }

    double str2double(const string& str) {
        // TODO str2double
        return 0;
    }

    int str2int(const string& str) {
        // TODO str2int
        return 0;
    }
}
