//
// Created by hhk on 18-12-17.
//

#ifndef URLCODER_H
#define URLCODER_H

#include <bits/stdc++.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

namespace WebServer {
    using std::string;

    // linux c 一个中文占3个字节，使用utf-8编码; windows 一个中文2个字节,使用gb2312编码
    static int charset_convert(const char *from_charset, const char *to_charset,
                               char *in_buf, size_t in_left, char *out_buf, size_t out_left);


    static int UTF8_2_GB2312(char *in_buf, size_t in_left, char *out_buf, size_t out_left);


    static int GB2312_2_UTF8(char *in_buf, size_t in_left, char *out_buf, size_t out_left);


    static unsigned char hexchars[] = "0123456789ABCDEF";

    /**
     * 16进制转10进制
     * @param s
     * @return
     */
    static int htoi(const char *s);


    /**
     * utf-8编码转化为url格式(如果不是utf-8请使用GB2312_2_UTF8进行转换
     * @param s
     * @return
     */
    static string url_encode(const char *s, const char* encoding="utf-8");

    static string url_encode(string &s, const char* encoding="utf-8");

    /**
     * utf-8格式的url转化为utf-8格式的字符串
     * @param str
     * @return
     */
    string url_decode(const string &str, const char* encoding="utf-8");
}
#endif //URLCODER_H
