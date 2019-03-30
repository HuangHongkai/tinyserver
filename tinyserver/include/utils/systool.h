//
// Created by hhk on 18-12-19.
//

#ifndef SYSTOOL_H
#define SYSTOOL_H

//#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <cstdint>

namespace WebServer {
    /**
     * https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
     * Obtain a backtrace and print it to stdout.(
     */
    void print_trace(void);

#define throwException(Exception, ...) { \
        cerr << "[" << #Exception << "] : FILE: " << string(__FILE__).substr(string(__FILE__).find_last_of('/') + 1) << "   LINE: " << __LINE__ <<  "  FUNCTION: " <<__FUNCTION__ <<endl; \
        throw Exception(__VA_ARGS__); \
    }

    /**
     * https://stackoverflow.com/questions/6403803/how-to-get-backtrace-function-line-number-on-solaris
     * @param pc
     * @param sig
     * @param usrarg
     * @return
     */
    int walker(uintptr_t pc, int sig, void *usrarg);

    void printstack();
}
#endif //SYSTOOL_H
