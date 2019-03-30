//
// Created by hhk on 18-12-18.
//
#include "http/http_parser/HttpParser.h"

bool WebServer::HttpParser::parse_parm(const string &data, WebServer::HttpHeader &out) {
    if(data.empty()) return true;
    auto items = split(data, "&");
    for(auto& item: items) {
        auto keyvalue = split2(item, "=");
        out.set_request_parm(url_decode(keyvalue.first), url_decode(keyvalue.second));
    }
    return true;
}

bool WebServer::HttpParser::parse_form_data(const string &data, WebServer::HttpHeader &out) {
    if(data.empty()) return true;
    auto items = split(data, "&");
    for(auto& item: items) {
        auto keyvalue = split2(item, "=");
        out.set_form_data(url_decode(keyvalue.first), url_decode(keyvalue.second));
    }
    return true;
}

bool WebServer::HttpParser::parse(const char *data, WebServer::HttpHeader &out) {
    stringstream ss(data);
    string line;
    /* 处理第一行 */
    getline(ss, line);
    static regex reg("^(\\w{3,10}) (/[^\\?]*)[\\?]?([^\\?]*) HTTP/\\d{1,2}\\.\\d{1,2}\r$");
    smatch result;
    if(regex_match(line, result, reg)) {
        out.set_action(result[1]);
        out.set_url(result[2]);
        for(size_t i = 0 ;i <result.size();i++)
            //  cout << i << "  " << result[i] << endl;
            if(!parse_parm(result[3], out))
                return false;
    }else {
        cout << "fail" << endl;
        return false;
    }
    /* 处理头部 */
    size_t pos;
    string key, value;
    while(getline(ss, line)) {
        if(line=="\r") break;
        pos = line.find(": ");
        if(pos == string::npos) continue;
        key = line.substr(0, pos);
        trim(key);
        value = line.substr(pos+2);
        trim(value);
        out.set_attribute(key, value);
    }

    /* 处理post请求的数据 */
    if(out.get_attribute("Content-Type").find("application/x-www-form-urlencoded") != string::npos) {
        string form_data;
        string tmp_str;
        while(ss >> tmp_str)
            form_data.append(tmp_str);
        parse_form_data(form_data, out);
    }
    return true;
}
