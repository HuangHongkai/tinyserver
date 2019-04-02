//
// Created by hhk on 18-12-14.
//

#include "tinyserver.h"
#include "ejson.h"

using namespace WebServer;
using namespace EasyJson;

void render_string(HttpRequest &request, HttpResponse &response) {
    response.set_header_attribute("key1", "value1")
            .set_header_attribute("key2", "value2")
            .set_response_code(404)
            .set_data(R"(<center><big><p>my web server</p><p><a href="download">download file</a>&nbsp;&nbsp;&nbsp;<a href="json">json</a>&nbsp;&nbsp;&nbsp;<a href="text">plain text</a>&nbsp;&nbsp;&nbsp;<a href="html">html</a>&nbsp;&nbsp;&nbsp;<a href="/re/12312423532">url re test</a>&nbsp;&nbsp;&nbsp;<a href="/tmpl?a=1234&b=sdfg">template</a></p></big></center>)");
}

void download_file(HttpRequest &request, HttpResponse &response) {
    response.set_file("/home/hhk/login.py")
            .set_content_type(STREAM);
}

void render_plain_text(HttpRequest &request, HttpResponse &response) {
    FILE *f = fopen("file", "rb");
    char buf[2048];
    if(f == nullptr)
        throw Exception404("文件名为 file 的文件不存在");
    size_t r = fread(buf, sizeof(char), 2048, f);
    response.set_data(buf, r)
            .set_content_type(PLAIN);
}

void render_html(HttpRequest &request, HttpResponse &response) {
    if(request.get_method() == GET ) {
        response.set_file("client.html");
    } else if(request.get_method() == POST) {
        const char * tmpl = R"(
            <html>
            <head>
                <meta charset="UTF-8"/>
                <title>你提交的信息 </title>
            </head>
            <p>ip地址和端口 %s: %d</p>
            <p>你提交的 username: %s </p>
            <p>你提交的 password: %s </p>
            <p><a href="/html">返回</a></p>
            </html>
        )";
        char *buf = (char*)malloc(strlen(tmpl) + 60);
        sprintf(buf, tmpl,
                request.get_ip(),
                request.get_port(),
                request.get_data("username").c_str(),
                request.get_data("password").c_str());
        response.set_data(buf);
        free(buf);
    }
}

void render_json(HttpRequest &request, HttpResponse &response) {
    string data = request.get_data("json");
    string indent_str = request.get_data("indent");
    int indent = 4;
    if(!indent_str.empty()) indent = indent_str[0]-'0';
    if(!data.empty()) {
        try {
            JSONBase *json = JSONBase::parse(data);
            response.set_data(json->toString(indent)).set_content_type(JSON);
        }catch(JSONParseException& e) {
            response.set_data("<h1>json解析错误</h1>");
        }
        return;
    }
    // obj初始化方法
    JSONObject obj = {
            {"test obj", {"key", "value"}},
            {"int number", 2},
            {"float number", 34.3},
            {"my string", "fsdfsdg"},
            {"nullptr", nullptr},
            {"emptylist", {}},
            {"列表嵌套列表", {1,2.3, "sdf", {1,2.242, "float"}}}, // 列表嵌套
            {"列表嵌套对象和列表", {
                {{"key1", 1234}}, // 被认为是对象
                {"key1", 1234}, // 被认为是列表
                1.23,
                234325
            }},
            {"对象", { // 所有的值都是{key, value}对则认为是嵌套对象
                {"a obj", 1234566},
                {"b obj", "b obj value"},
            }}
    };
    obj["int"] = 12325;
    obj["float"] = 24235.235235;
    obj["string"] = "this is a string2";
    obj["null"] = nullptr;

    obj.add("int", 123) // 整数
       .add("float", 23.2324) // 浮点数
       .add("string", "this is a string") // 英文字符串
       .add("中文", "这是中文字符串。。。") // 中文字符串
       .add("symbols  \",.[]@!#$%^", "含有特殊符号\"") // 特殊符号
       .add("list", {1, 2.34, "english string", 123.4, "中文字符串"}) // 列表
       .add("this is null", nullptr) // null测试
       .add("object", {{"key1", "value1"}, {"key2", 223}, {"key3", 23.4}}); // 对象

    JSONArray arr = {123.4, "dfsdf", 23, nullptr, {14.3, "ffa", 124}};
    arr[0] = 12.314;
    arr[2] = 2335;
    arr[3] = "字符串";
    arr[6] = obj;  // json对象
    arr.add(123456) // 整数
       .add(234.235236) //浮点数
       .add("字符串") //字符串
       .add(nullptr); // null测试

    obj.add("list2", arr); //加入arr对象

    response.set_data(obj.toString(2))
            .set_content_type(JSON);
}

void re_url_test(HttpRequest &request, HttpResponse &response) {
    auto match = request.get_url_parm();
    stringstream ss;
    ss << R"(<html><head><meta charset="charset=UTF-8"\></head>)";
    ss << R"(<p>匹配到的数字为: )" << match[0];
    ss << R"(</p>)";
    ss << "</html>";
    response.set_data(ss.str());
}

void render_tmpl(HttpRequest &request, HttpResponse &response) {
//    JSONObject context = {
//            {"username", "hhk"},
//            {"obj", {
//                {"abcd", {1,2.3}},
//                {"obj", {
//                    {"key", "this is obj.obj.key 's value"},
//                }},
//            }},
//            {"list", {1, "qwerty", 2.34, {
//                {"key", "this is a key"},
//            }}},
//    };
    JSONObject context = OBJECT(
            KEYVALUE("username", "hhk"),
            KEYVALUE("obj", OBJECT(
                    KEYVALUE("abcd", LIST(1,2.3, "hahahaha")),
                    KEYVALUE("obj", OBJECT(
                            KEYVALUE("key", "this is obj.obj.key' s value")
                    ))
            )),
            KEYVALUE("null", nullptr),
            KEYVALUE("list", LIST(1, "qwerty", 2.34, OBJECT(
                            KEYVALUE("key", "this is a key"),
                            KEYVALUE("key2", "this is a key2"),
                    ))),
            KEYVALUE("list1", LIST(1,2,3,4,5)),
            KEYVALUE("list2", LIST(1,2,3)),
            KEYVALUE("obj2", OBJECT(
                    KEYVALUE("key1", "value1"),
                    KEYVALUE("key2", 222),
                    KEYVALUE("key3", 333),
            )),
            KEYVALUE("a", "111"),
            KEYVALUE("b", "222"),
            KEYVALUE("c", "333"),
    );
    context.add("parm", request.get_request_parm());
    render(response, "tmpl.html", context);
}

int main() {
    IHttpServer *server = new BaseHttpServer();
    server->enrol("/", render_string);
    server->enrol("/download", download_file);
    server->enrol("/text", render_plain_text);
    server->enrol("/html", render_html);
    server->enrol("/json", render_json);
    server->enrol("/tmpl", render_tmpl);
    server->enrol("/re/(\\d+)", re_url_test);
    server->run();
}

