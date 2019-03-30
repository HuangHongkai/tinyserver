//
// Created by hhk on 18-12-19.
//

#include <utils/systool.h>

#include "utils/systool.h"

void WebServer::print_trace() {
//    void *array[10];
//    size_t size;
//    char **strings;
//    size_t i;
//
//    size = static_cast<size_t>(backtrace (array, 5));
//    strings = backtrace_symbols (array, static_cast<int>(size));
//
//    fprintf (stderr, "Obtained %zd stack frames.\n", size);
//
//    for (i = 0; i < size; i++)
//        fprintf (stderr, "%s\n", strings[i]);
//
//    free (strings);
}


int WebServer::walker(uintptr_t pc, int sig, void *usrarg) {
//    Dl_info dlip;
//    if(dladdr((void *)pc, &dlip)) {
//        (void)printf(" %08lx %s %s\n", pc, dlip.dli_fname, dlip.dli_sname);
//        return 0;
//    } else {
//        perror("dladdr()");
//        return -1;
//    }
}

void WebServer::printstack() {
    ucontext_t ucp;
    if(getcontext(&ucp)) {
        perror("getcontext()");
        return;
    } else {
        (void)printf("\nwalkcontext()\n");
     //   return walkcontext(&ucp, &walker, NULL);

    }

}
