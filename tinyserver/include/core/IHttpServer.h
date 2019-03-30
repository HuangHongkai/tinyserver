//
// Created by hhk on 18-12-14.
//

#ifndef IHTTPSERVER_H
#define IHTTPSERVER_H
#include "http/HttpObject.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"

namespace WebServer {
    using std::regex;

    class IHttpServer {
    public:
        typedef void (*VIEW_HANDLER)(HttpRequest&, HttpResponse&);

        typedef void (*ERROR_HANDLER)(HttpResponse&, const string&);

        virtual void run() = 0;

        virtual bool enrol(const string& url_rule, VIEW_HANDLER func) = 0;
    };
}

#endif //IHTTPSERVER_H
