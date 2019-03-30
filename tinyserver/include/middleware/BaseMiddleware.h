//
// Created by hhk on 18-12-23.
//

#ifndef MYWEBSERVER_BASEMIDDLEWARE_H
#define MYWEBSERVER_BASEMIDDLEWARE_H

#include "http/HttpResponse.h"
#include "http/HttpRequest.h"
// TODO BaseMiddleware
// 中间件基类，有process_request和process_response
namespace WebServer {
    class BaseMiddleware {
    public:
        virtual HttpResponse& process_request(HttpRequest& request) = 0;
        virtual HttpResponse& process_response(HttpResponse& response) = 0;
    };
}


#endif //MYWEBSERVER_BASEMIDDLEWARE_H
