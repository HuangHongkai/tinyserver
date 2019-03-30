//
// Created by hhk on 18-12-18.
//

#include "http/HttpHeader.h"

string WebServer::HttpHeader::get_code_str() {
    if (code == 200)
        return "OK";
    else if (code == 401)
        return "Unauthorized";
    else if (code == 403)
        return "Forbidden";
    else if (code == 404)
        return "Not Found";
    else if (code == 500)
        return "Internal Server Error";
    else if (code == 502)
        return "Bad Gateway";
    else if (code == 503)
        return "Service Unavailable";
    else if (code == 504)
        return "Gateway Timeout";
    return "OK";
}

WebServer::HttpHeader &WebServer::HttpHeader::set_code(WebServer::StatusCode code) {
    if (code == SUCCESS)
        this->code = 200;
    else if (code == UNAUTHORIZED)
        this->code = 401;
    else if (code == FORBIDDEN)
        this->code = 403;
    else if (code == NOT_FOUND)
        this->code = 404;
    else if (code == INTERNAL_SERVER_ERROR)
        this->code = 500;
    else if (code == BAD_GATEWAY)
        this->code = 502;
    else if (code == SERVICE_UNAVAILABLE)
        this->code = 503;
    else if (code == GATEWAY_TIMEOUT)
        this->code = 504;
    return *this;
}

string WebServer::HttpHeader::to_string() {
    char res[1000];
    sprintf(res, "HTTP/1.1 %d %s\r\n", code, get_code_str().c_str());
    for (auto item: attributes) {
        if(item.second.empty()) continue;
        sprintf(res + strlen(res), "%s:%s\r\n", item.first.c_str(), item.second.c_str());
    }
    sprintf(res + strlen(res), "\r\n");
    return res;
}

WebServer::HttpHeader &WebServer::HttpHeader::set_form_data(const string &key, const string &value) {
    request_form_data[key] = value;
    return *this;
}
