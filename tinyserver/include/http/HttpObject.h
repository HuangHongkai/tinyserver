//
// Created by hhk on 18-12-14.
//

#ifndef HTTPOBJECT_H
#define HTTPOBJECT_H
#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

namespace WebServer {
    enum ContentType {
        HTML,
        JSON,
        IMAGE_PNG,
        IMAGE_JPG,
        PLAIN,
        STREAM,
    };
    enum StatusCode {
        SUCCESS,
        UNAUTHORIZED,
        FORBIDDEN,
        NOT_FOUND,
        INTERNAL_SERVER_ERROR,
        BAD_GATEWAY,
        SERVICE_UNAVAILABLE,
        GATEWAY_TIMEOUT
    };

    enum ResponseType {
        STRING_PLAIN,
        BINARY_DATA,
        BINARY_FILE,
    };

    enum HttpMethod {
        UNKNOWN,
        GET,
        POST,
        PUT,
        DELETE,
        HEAD,
        OPTIONS,
    };
}
#endif //HTTPOBJECT_H
