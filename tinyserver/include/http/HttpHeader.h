//
// Created by hhk on 18-12-18.
//

#ifndef HTTPHEADER_H
#define HTTPHEADER_H
#include "HttpObject.h"
#include "utils/StringUtils.h"

namespace WebServer {
    class HttpHeader {
        int code {};
        map<string, string> attributes;
        string url;
        string action;
        map<string, string> request_parm_data;
        map<string, string> request_form_data;

        string get_code_str();

    public:
        HttpHeader() = default;

        HttpHeader &set_code(int code) {
            this->code = code;
            return *this;
        }

        int get_code() { return code; }

        HttpHeader &set_code(StatusCode code);

        HttpHeader &set_url(const string& url) {
            this->url = url;
            return *this;
        }

        string& get_url() { return this->url; }

        HttpHeader &set_action(const string& action) {
            this->action = action;
            return *this;
        }

        string& get_action() { return this->action; }

        HttpHeader &set_attribute(const string &key, const string &value) {
            str_tolower(const_cast<string&>(key));
            attributes[key] = value;
            return *this;
        }

        string get_attribute(const string &key) {
            str_tolower(const_cast<string&>(key));
            auto it = attributes.find(key);
            if(it == attributes.end()) return string();
            return it->second;
        }

        string get_request_parm(const string& key) {
            auto it = request_parm_data.find(key);
            if(it == request_parm_data.end()) return string();
            return request_parm_data[key];
        }

        map<string, string>& get_request_parm() { return request_parm_data; }

        HttpHeader &set_request_parm(const string& key, const string& value) {
            request_parm_data[key] = value;
            return *this;
        }

        string get_form_data(const string& key) {
            auto it = request_form_data.find(key);
            if(it == request_form_data.end()) return string();
            return request_form_data[key];
        }

        map<string, string>& get_form_data() { return request_form_data; }

        HttpHeader &set_form_data(const string& key, const string& value);

        string to_string();
    };
}

#endif //HTTPHEADER_H
