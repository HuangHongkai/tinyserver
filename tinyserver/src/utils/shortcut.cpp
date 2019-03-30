//
// Created by hhk on 18-12-19.
//

#include "utils/shortcut.h"
#include "tinytemplate.h"
#include "core/BaseException.h"
#include "utils/systool.h"

void WebServer::render(WebServer::HttpResponse &response, const string &path, const EasyJson::JSONObject &context) {
    if(access(path.c_str(), F_OK) != 0) 
        throwException(Exception404, "模板不存在");
    TinyTemplate::HtmlTemplate html(path);
    html.setValue(context);
    response.set_data(html.render());
}
