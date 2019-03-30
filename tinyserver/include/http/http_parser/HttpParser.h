//
// Created by hhk on 18-12-17.
//

#ifndef HTTPPARSER_H
#define HTTPPARSER_H

//#include <iconv.h>
#include "UrlCoder.h"
#include "utils/StringUtils.h"
#include "http/HttpHeader.h"

namespace WebServer{
    using std::regex;

    class HttpParser{
    public:
        static bool parse_parm(const string& data, HttpHeader& out);

        static bool parse_parm(string& data, HttpHeader& out) { return parse_parm((const string&)data, out); }

        static bool parse_form_data(string& data, HttpHeader& out) { return parse_form_data((const string&)data, out); }

        static bool parse_form_data(const string& data, HttpHeader& out);

        static bool parse(const char* data, HttpHeader& out);

        static void main() {
            const char* s = "GET /sdfgh/sdfsd/?gsdg=asdfsdfs&asdf=sdf HTTP/1.1\r\nAccept: */*\r\nCache-Control: no-cache\r\nHost: localhost:8888\r\nConnection: Keep-Alive\r\nUser-Agent: Apache-HttpClient/4.5.5 (Java/1.8.0_152-bin)\r\nAccept-Encoding: gzip,deflate\r\n\r\n";
            HttpHeader out;
            parse(s, out);
            const char* s2 = "POST / HTTP/1.1\r\n"
                             "Accept: */*\r\n"
                             "Cache-Control: no-cache\r\n"
                             "Content-Length: 108\n"
                             "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n"
                             "Host: localhost:8888\r\n"
                             "Connection: Keep-Alive\r\n"
                             "User-Agent: Apache-HttpClient/4.5.5 (Java/1.8.0_152-bin)\r\n"
                             "Accept-Encoding: gzip,deflate\r\n"
                             "\r\n"
                             "sdg=%22fsdf%26%26%26%3D%26&%E6%88%91%E7%94%9F%E6%97%A5v=&a=sdfdf";
            HttpHeader out2;
            parse(s2, out2);
        }
    };
}

#endif //HTTPPARSER_H
