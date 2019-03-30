//
// Created by hhk on 18-12-13.
//

#include "utils/StringUtils.h"
#include <ctype.h>

namespace WebServer {
    using std::transform;

    string itos( const long i, const ios_base::fmtflags base) {
        char format[] = "%ld";
        if ( base == std::ios_base::hex )
            strcpy( format, "%o" );
        else if ( base == ios_base::oct )
            strcpy( format, "%X" );

        size_t len = 33;
        char *buf = new char[len];
        memset( buf, 0, len );
        snprintf( buf, len, format, i );
        string result(buf);
        delete[] buf;
        return result;
    }

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

    std::vector<std::string> split(const std::string &input, const string &delim) {
        std::regex re(delim);
        std::sregex_token_iterator p(input.begin(), input.end(), re, -1);
        std::sregex_token_iterator end;
        std::vector<std::string> vec;
        while (p != end)
            vec.emplace_back(*p++);
        return vec;
    }

    string &str_tolower(string &str) {
        for(auto& i: str)
            i = static_cast<char>(tolower(i));
        return str;
    }

    string &str_toupper(string &str) {
        for(auto& i: str)
            i = static_cast<char>(toupper(i));
        return str;
    }

    pair<string, string> split2(const string &data, const string &delim) {
        size_t pos = data.find(delim);
        if(pos != string::npos) {
            return {data.substr(0, pos), data.substr(pos+delim.size())};
        }
        return {data, ""};
    }

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
}