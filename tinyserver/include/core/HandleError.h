//
// Created by hhk on 18-12-18.
//

#ifndef HANDLEERROR_H
#define HANDLEERROR_H
#include "http/HttpResponse.h"
#include "http/HttpRequest.h"

namespace WebServer{
#define GetErrorHandlerName(code) default_handler_##code

#define GenErrorHandler(code) \
    static void default_handler_##code(HttpResponse& response, const string& message = ""){ \
        char buf[200]; \
        sprintf(buf, "<center><h1>"#code"</h1><p>%s</p><p>Created by hhk</p></center>", message.c_str()); \
        response.set_data(buf).set_response_code(code); \
    }

#define RegisterErrorHandler(code) handle_##code = default_handler_##code

    GenErrorHandler(401);

    GenErrorHandler(403);

    GenErrorHandler(404);

    GenErrorHandler(500);

    GenErrorHandler(501);

    GenErrorHandler(502);

    GenErrorHandler(503);

#undef GenErrorHandler
}
#endif //HANDLEERROR_H
