//
// Created by hhk on 18-12-18.
//

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <arpa/inet.h>
#include "HttpHeader.h"
#include "utils/any.hpp"

namespace WebServer {
    class HttpRequest {
    private:
        map<string, any> attributes;
        HttpHeader req_header;
        HttpMethod method = UNKNOWN;
        in_addr address;
        in_port_t port;
    public:
        HttpRequest &parseRequest(const char *data);

        HttpHeader &get_header() { return req_header; }

        HttpRequest &set_header(const HttpHeader &header) {
            this->req_header = header;
            return *this;
        }

        string get_request_parm(const string& key) {
            return req_header.get_request_parm(key);
        }

        map<string, string>& get_request_parm() {
            return req_header.get_request_parm();
        }

        string get_form_data(const string& key) {
            return req_header.get_form_data(key);
        }

        map<string, string>& get_form_data() {
            return req_header.get_form_data();
        }

        char* get_ip() { return inet_ntoa(address); }

        int get_port() { return port; }

        HttpRequest &set_ip(in_addr addr) { this->address = addr; return *this; }

        HttpRequest &set_port(in_port_t port) { this->port = ntohs(port); return *this; }

        HttpMethod get_method();

        string get_data(const string& key) {
            get_method();
            if(method == POST)
                return req_header.get_form_data(key);
            else if(method != UNKNOWN)
                return req_header.get_request_parm(key);
            return "";
        }

        string get_url() { return req_header.get_url(); }

        HttpRequest &set_attributes(const string& key, const any& obj) { attributes[key] = obj; }

        template<typename T>
        T& get_attributes(const string& key) { return any_cast<T>(attributes[key]); }

        vector<string> get_url_parm() {
            auto it = attributes.find("server_url_parm");
            if(it != attributes.end())
                return any_cast<vector<string>>(it->second);
            return {};
        }

        HttpRequest &set_url_parm(vector<string> &parm) {
            attributes["server_url_parm"] = parm;
            return *this;
        }
    };
}
#endif //HTTPREQUEST_H
