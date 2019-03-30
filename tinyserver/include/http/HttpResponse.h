//
// Created by hhk on 18-12-18.
//

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include "HttpHeader.h"
#include "core/BaseException.h"

namespace WebServer{
    class HttpResponse {
        HttpHeader res_header;
        char *data_buf;
        char *header_buf;
        FILE *f;
        size_t data_size;
        size_t header_size;
        const size_t MAX_SIZE = 4096 * 1024;  // 包最大的大小，4MB
        const size_t PADDING_SIZE = 0; // 尾部填充大小
        ResponseType responseType;
    private:
        void _set_data(const string &data);

        void _set_file(const string &path);

        char *_get_header();

    public:
        HttpResponse();

        HttpResponse &set_content_type(const string &content_type) {
            res_header.set_attribute("Content-Type", content_type);
            return *this;
        }

        HttpResponse &set_content_type_charset(const string& charset = "UTF-8");

        HttpResponse &enable_gzip();

        HttpResponse &set_content_type(ContentType contentType);

        size_t get_frame_size() {
            return header_size + data_size;
        }

        HttpResponse &set_header_attribute(const string& key, const string& value) {
            res_header.set_attribute(key, value); // fix ambiguous bug
            return *this;
        }

        HttpResponse &set_response_code(int code) {
            res_header.set_code(code);
            return *this;
        }

        int get_response_code() { return res_header.get_code(); }

        HttpResponse &set_data(const string &data) {
            _set_data(data);
            return *this;
        }

        HttpResponse &set_data(string &&data) {
            _set_data(data);
            return *this;
        }

        HttpResponse &set_data(char *data, size_t size);

        FILE *get_file() {
            return f;
        }

        HttpResponse &set_file(FILE *f);

        HttpResponse &set_file(const string &path) {
            _set_file(path);
            return *this;
        }

        void get_header_buf(void *&buf, size_t &len) {
            _get_header();
            massert(header_buf != nullptr);
            len = header_size;
            buf = header_buf;
        }

        void get_data_buf(void *&buf, size_t &len) {
            massert(data_buf != nullptr);
            len = data_size;
            buf = data_buf;
        }

        void write_to_fd(int fd);

        ~HttpResponse();
    };
}


#endif //HTTPRESPONSE_H
