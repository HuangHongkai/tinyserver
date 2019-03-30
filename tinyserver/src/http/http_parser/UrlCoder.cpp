//
// Created by hhk on 18-12-19.
//

#include "http/http_parser/UrlCoder.h"

int WebServer::charset_convert(const char *from_charset, const char *to_charset, char *in_buf, size_t in_left,
                               char *out_buf, size_t out_left) {
    iconv_t icd;
    size_t sRet;
    char *pIn = in_buf;
    char *pOut = out_buf;
    size_t outLen = out_left;

    if (NULL == from_charset || NULL == to_charset || NULL == in_buf || 0 >= in_left || NULL == out_buf ||
        0 >= out_left) {
        return -1;
    }

    icd = iconv_open(to_charset, from_charset);
    if ((iconv_t) -1 == icd) {
        return -1;
    }

    sRet = iconv(icd, &pIn, &in_left, &pOut, &out_left);
    if ((size_t) -1 == sRet) {
        iconv_close(icd);
        return -1;
    }

    out_buf[outLen - out_left] = 0;
    iconv_close(icd);
    return (int) (outLen - out_left);
}

int WebServer::UTF8_2_GB2312(char *in_buf, size_t in_left, char *out_buf, size_t out_left) {
    return charset_convert("UTF-8", "GB2312", in_buf, in_left, out_buf, out_left);
}

int WebServer::GB2312_2_UTF8(char *in_buf, size_t in_left, char *out_buf, size_t out_left) {
    return charset_convert("GB2312", "UTF-8", in_buf, in_left, out_buf, out_left);
}

int WebServer::htoi(const char *s) {
    int value;
    int c;

    c = ((unsigned char *) s)[0];
    if (isupper(c))
        c = tolower(c);
    value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

    c = ((unsigned char *) s)[1];
    if (isupper(c))
        c = tolower(c);
    value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

    return (value);
}

std::string WebServer::url_encode(const char *s, const char *encoding) {
    // TODO 处理gb2312
    unsigned char c;
    size_t len = strlen(s);
    unsigned char *to, *start;
    unsigned char const *from, *end;

    from = (unsigned char *) s;
    end = (unsigned char *) s + len;
    start = to = (unsigned char *) calloc(1, 3 * len + 1);

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
        } else if ((c < '0' && c != '-' && c != '.') ||
                   (c < 'A' && c > '9') ||
                   (c > 'Z' && c < 'a' && c != '_') ||
                   (c > 'z')) {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
        } else {
            *to++ = c;
        }
    }
    *to = 0;
    size_t out_length = to - start;
    string res((char*)start, out_length);
    free(start);
    return res;
}

std::string WebServer::url_encode(std::string &s, const char *encoding) {
    return url_encode(s.c_str(), encoding);
}

std::string WebServer::url_decode(const std::string &str, const char *encoding) {
    // TODO 处理gb2312
    const char *data = str.c_str();
    size_t len;
    char *dest, *p;
    len = strlen(data);
    dest = new char[len];
    p = dest;
    while (len--) {
        if (*data == '+') {
            *p = ' ';
        } else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2))) {
            *p = (char)htoi(data + 1);
            data += 2;
            len -= 2;
        } else {
            *p = *data;
        }
        data++;
        p++;
    }
    *p = 0;
    size_t out_len = p - dest;
    string res(dest, out_len);
    free(dest);
    return res;
}
