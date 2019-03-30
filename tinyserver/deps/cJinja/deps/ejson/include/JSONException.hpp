//
// Created by hhk on 18-12-17.
//

#ifndef JSONEXCEPTION_H
#define JSONEXCEPTION_H
#include <bits/stdc++.h>
#include <utility>

namespace EasyJson {
    using std::exception;
    using std::string;
    using std::cerr;
    using std::endl;

#define throwException(Exception, ...) { \
        std::cerr << "[" << #Exception << "] : FILE: " << string(__FILE__).substr(string(__FILE__).find_last_of('/') + 1) << "   LINE: " << __LINE__ <<  "  FUNCTION: " <<__FUNCTION__ << std::endl; \
        throw Exception(__VA_ARGS__); \
    }

    class JSONParseException : exception {
    private:
        string msg;
    public:
        explicit JSONParseException(string& msg) : msg(msg) {}

        explicit JSONParseException(string msg) : msg(std::move(msg)) {}

        JSONParseException() : msg("JSONParseException") {}

        char const *what() const noexcept override {
            return msg.c_str();
        }
    };
}
#endif //JSONEXCEPTION_H
