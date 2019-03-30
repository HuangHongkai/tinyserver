//
// Created by hhk on 18-12-19.
//

#ifndef SHORTCUT_H
#define SHORTCUT_H
#include "http/HttpResponse.h"
#include "ejson.h"

/**
 * 封装基础api
 */
namespace WebServer {
    // 注意： 此函数会调用移动构照函数将context的内容全部移走
    void render(HttpResponse &response, const string &path, const EasyJson::JSONObject &context = {});
}
#endif //SHORTCUT_H
