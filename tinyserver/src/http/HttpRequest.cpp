//
// Created by hhk on 18-12-18.
//

#include "http/HttpRequest.h"
#include "http/http_parser/HttpParser.h"

WebServer::HttpRequest &WebServer::HttpRequest::parseRequest(const char *data) {
    HttpParser::parse(data, req_header);
    return *this;
}

WebServer::HttpMethod WebServer::HttpRequest::get_method() {
    if(method == UNKNOWN) {
        string& action = str_tolower(req_header.get_action());
        if(action == "get")
            method = GET;
        else if(action == "post")
            method = POST;
        else if(action == "put")
            method = PUT;
        else if(action == "head")
            method = HEAD;
        else if(action == "options")
            method = OPTIONS;
        else if(action == "delete")
            method = DELETE;
    }
    return method;
}
