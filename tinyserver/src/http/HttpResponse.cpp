//
// Created by hhk on 18-12-18.
//

#include "http/HttpResponse.h"
#include "core/BaseException.h"
#include "utils/common_tool.h"
#include "utils/systool.h"

WebServer::HttpResponse &WebServer::HttpResponse::set_content_type(WebServer::ContentType contentType) {
    if (contentType == HTML)
        res_header.set_attribute("Content-Type", "text/html;charset=UTF-8");
    else if(contentType == JSON)
        res_header.set_attribute("Content-Type", "application/json;charset=UTF-8");
    else if (contentType == IMAGE_JPG)
        res_header.set_attribute("Content-Type", "image/jpg");
    else if (contentType == IMAGE_PNG)
        res_header.set_attribute("Content-Type", "image/png");
    else if (contentType == PLAIN)
        res_header.set_attribute("Content-Type", "text/plain;charset=UTF-8");
    else if (contentType == STREAM)
        res_header.set_attribute("Content-Type", "application/octet-stream");
    return *this;
}

WebServer::HttpResponse &WebServer::HttpResponse::set_data(char *data, size_t size) {
    if (size > MAX_SIZE)
        throwException(FileException, "data too big!");
    data_size = size;
    data_buf = (char *) malloc(data_size);
    memcpy(data_buf, data, size);
    responseType = BINARY_DATA;
    return *this;
}

void WebServer::HttpResponse::write_to_fd(int fd) {
    void *buf1, *buf2;
    size_t len1, len2;

    /*get header*/
    get_header_buf(buf1, len1);

    write(fd, buf1, len1); // write header
    /*write data*/
    if(responseType == BINARY_FILE) {
        long read_size = get_file_size(f);
        if(read_size > 0 && read_size <= MAX_SIZE) {
            char *tmp = new char[read_size];
            fread(tmp, sizeof(char), static_cast<size_t>(read_size), f);
            write(fd, tmp, static_cast<size_t>(read_size));
            free(tmp);
        }
    } else {
        get_data_buf(buf2, len2);
        write(fd, buf2, len2);
    }
}

WebServer::HttpResponse &WebServer::HttpResponse::set_content_type_charset(const string &charset) {
    string content_type = res_header.get_attribute("Content-Type");
    if(!content_type.empty() && content_type.find("; charset=") == string::npos) {
        content_type.append(";  charset="+charset);
        res_header.set_attribute("Content-Type", content_type);
    }
    return *this;
}

WebServer::HttpResponse::HttpResponse() {
    header_buf = nullptr;
    data_buf = nullptr;
    f = nullptr;
    header_size = data_size = 0;
    responseType = STRING_PLAIN;
    set_content_type(ContentType::HTML);
    res_header.set_code(StatusCode::SUCCESS);
    set_content_type_charset();
}

char *WebServer::HttpResponse::_get_header() {
    string header = res_header.to_string();
    if (header.length() > MAX_SIZE)
        throwException(FileException, "header too big");
    header_size = header.length();
    header_buf = (char *) malloc(header_size);
    memcpy(header_buf, header.c_str(), header_size);
    return header_buf;
}

void WebServer::HttpResponse::_set_file(const string &path) {
    if (f)
        fclose(f);
    f = fopen(path.c_str(), "rb");
    if(f == nullptr)
        throwException(Exception404, "你访问的 " + path + "不存在");;
    responseType = BINARY_FILE;
}

void WebServer::HttpResponse::_set_data(const string &data) {
    data_size = data.length() + PADDING_SIZE;
    if (data_size > MAX_SIZE)
        throwException(FileException, "data too big");;
    this->data_buf = (char *) malloc(data_size);
    memcpy(this->data_buf, data.c_str(), data.length());
}

WebServer::HttpResponse &WebServer::HttpResponse::enable_gzip() {
    // 启用gzip压缩
    //res_header.set_attribute("Vary", "Accept-Encoding");
    //res_header.set_attribute("Content-Encoding", "gzip");
    return *this;
}

WebServer::HttpResponse &WebServer::HttpResponse::set_file(FILE *f) {
    if (this->f)
        fclose(this->f);
    this->f = f;
    responseType = BINARY_FILE;
    return *this;
}

WebServer::HttpResponse::~HttpResponse() {
    if (f)
        fclose(f);
    if (data_buf)
        free(data_buf);
    if (header_buf)
        free(header_buf);
}
