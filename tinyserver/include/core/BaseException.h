#include <utility>

//
// Created by hhk on 18-12-14.
//

#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <bits/stdc++.h>

using namespace std;

namespace WebServer {
    class BaseException : public exception {
    private:
        string msg;
    public:
        explicit BaseException(string msg) : msg(std::move(msg)) { }

        BaseException() = default;

        virtual char const *what() const noexcept override {
            return msg.c_str();
        }

        void set_msg(const string &msg) {
            this->msg = msg;
        }
    };

    class AssertException : BaseException {
    public:
        AssertException() { set_msg("AssertException"); }
    };

    template<typename T>
    void massert(T cond) {
        if (cond)
            return;
        throw AssertException();
    }

    class FileException: public BaseException {
    public:
        explicit FileException(const string& msg): BaseException(msg) { }
        FileException() { set_msg("文件读写错误"); }
    };

    class Exception404 : public BaseException {
    public:
        explicit Exception404(const string& msg):BaseException(msg) {  }
        Exception404() { set_msg("404 exception"); }
    };

    class Exception500 : public BaseException {
    public:
        explicit Exception500(const string& msg):BaseException(msg) {  }
        Exception500() { set_msg("500 exception"); }
    };
}
#endif //BASEEXCEPTION_H
