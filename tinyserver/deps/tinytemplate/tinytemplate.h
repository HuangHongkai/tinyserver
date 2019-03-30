//
// Created by hhk on 18-12-19.
//

#ifndef TINY_TEMPLATE_H
#define TINY_TEMPLATE_H

#include <stdio.h>
#include <bits/stdc++.h>
#include "ejson.h"

namespace TinyTemplate {
    using std::string;
    using std::stack;
    using std::regex;
    using std::smatch;
    using std::map;
    using std::vector;
    using std::exception;
    using std::pair;
    using EasyJson::JSONObject;
    using EasyJson::JSONArray;
    using std::stringstream;

    // TODO html简单模板渲染库
    /**
     * django jinja2模板语法
     * 基础:(已经全部完成)
     * 变量 {{ var }}
     * 变量下标访问(支持嵌套) {{ var.var2 }} {{ var[2] }} {{ var[2].key.value[2] }}
     * 表达式计算(包括字符串拼接) {{ 1*1+2-3*var }} {{ 1+1*2-3/4 }} {{ "asdfsf"+var }}
     * for-endfor语句(对于object，支持key-value迭代) {% for var in list %} {% endfor %}
     * if-endif语句 {% if condition %} {% endif %}
     * if-else-endif语句 {% if condtion %} {% else %} {% endif %}
     * 简单运算符 + - * /
     * 简单的逻辑操作 ! == !=
     * 模板包含 {% include 'other.html' %}
     *
     * 更多：
     * 模板继承 {% extends 'base.html' %} (相当麻烦，要实现几个内置的过滤器)
     * 继承模板块 {% block other %} {% endblock %}
     * 过滤器 {% var | filter %}  (比较麻烦，要实现几个内置的过滤器)
     * with语法 {% with var1.var2 as var2 %}{% endwithd %}  (很容易实现)
     *
     * 与jinja2的区别
     * 1. 取反使用!，而jinja2使用的是not (对应python的not)
     * 2. 表达式之间不能有空格(支持空格的实现其实不难，只是懒得在弄一个解析了，而且没有空格看起来也很清晰)
     */

    class HtmlTemplate {
        string tmpl;
        map<string, EasyJson::any> var_tables; // 变量映射表
        FILE* file = nullptr;
        const long MAX_FILE_SIZE = 1024 * 1024 * 4; // 4MB
        stringstream ss;

        /**
         * 计算器的实现程序，支持+ - * /
         * @param number 操作数列表
         * @param op 操作符号列表
         * @param ans 为了方便返回全都是浮点数
         */
        double calculator(vector<EasyJson::any>& number, vector<char>& op);

        /**
         * 解析并输出到 stringstream
         * @param start
         * @param len
         */
        void parse(size_t start, size_t len);

        /**
         * 解析变量,支持运算，例如 {{ 1+2 }},底层调用__parse_var
         * @param tmp_str 格式为{{ abcd+123+a.b.c[23] }}
         * @return
         */
        EasyJson::any parse_var(const string& tmp_str);

        /**
         * 解析变量，var 为变量名，从变量替换表中找到合适的值来替代，
         * @param var 格式 abcd.efg[123]
         * @return
         */
        EasyJson::any __parse_var(string& var);

        /**
         * 解析for语句 {% for var1 in var2 %}
         * @param start  起始位置
         * @param len  长度
         * @param var1
         * @param var2
         */
        void parse_for(size_t start, size_t len, const string& var1, const string& var2);

        /**
         * 解析if语句 {% if statement %}
         * @param start
         * @param len
         * @param statement  一个表达式，例如 {% if 1+1==2 %} ,注意表达式内部不能有空格
         */
        void parse_if(size_t start, size_t len, const string& statement);

        /**
         * 找到下一个匹配的位置,例如为{%for %} 语句找到 {%endfor%}的位置,为 {%if%} 语句找到{%endif%}
         * @param start
         * @param end
         * @param reg 开始位置的正着表达式
         * @param reg2 结束位置的正则表达式
         * @return
         * (注:由于for之间合if可以嵌套使用，所以需要传入2个正则表达式)
         */
        smatch findNext(string::const_iterator start, string::const_iterator end, const regex& reg, const regex& reg2);

        /**
         * 打印错误位置附近的字符串
         * @param pos 错误位置
         * @return
         */
        string errorView(size_t pos);
    public:
        explicit HtmlTemplate(const string& str, int flag = 0);

        HtmlTemplate& setValue(JSONObject&& obj) {
            var_tables = std::move(obj.getData());
            return *this;
        }

        /**
         * 设置变量替换表
         * @param obj
         * @return
         */
        HtmlTemplate& setValue(const JSONObject& obj) {
            var_tables = std::move(const_cast<JSONObject&>(obj).getData());
            return *this;
        }

        /**
         * 渲染
         * @return
         */
        string render();

        ~HtmlTemplate() {  if(file != nullptr) fclose(file); }
    };

    namespace Token {
        const char TMPL_START = '{';
        const char TMPL_END = '}';
        const char VAR_START = '{';
        const char VAR_END = '}';
        const char STATEMENT_START = '%';
        const char STATEMENT_END = '%';


        /**
         * {{ var }} {{ var.var[123] }}
         * {{ var*var2+1234 }} 简单运算
         * {{ var+"abcdefg" }} 字符串拼接
         */
        const regex VAR_REGEX(R"(\{\{\s*([\w"\[\]\.\+\*/\-]+)\s*\}\})");

        /**
         * {% for x in y %}
         */
        const regex FOR_REGEX(R"(\{%\s*for\s+([\w,]+)\s+in\s+([\w\[\]\.]+)\s*%\})");

        // {% endfor %}
        const regex ENDFOR_REGEX(R"(\{%\s*endfor\s*%\})");

        // {% if x %}
        const regex IF_REGEX(R"(\{%\s*if\s+([\w"\[\]\.\+\*/=!><\-]+)\s*%\})");

        // {% endif %}
        const regex ENDIF_REGEX(R"(\{%\s*endif\s*%\})");

        // { % else %}
        const regex ELSE_REGEX(R"(\{%\s*else\s*%\})");

        // {% include 'tmpl2.html' %}
        const regex INCLUDE_REGEX(R"(\{%\s*include\s*'([\w\.]+)'\s*%\})");

        // if表达式解析
        // abcd==efgh 匹配出abcd和efgh
        const regex EQUAL_REGEX(R"(([\w"\[\]\.\+\*/\-]+)(==|<|>)([\w"\[\]\.\+\*/\-]+))");

        // !abcdef 匹配出abcdef
        const regex NOT_REGEX(R"(!([\w"\[\]\.\+\*/\-]+))");
    };

    // 基础异常类
    class BaseException : public exception {
    private:
        string msg;
    public:
        explicit BaseException(string msg) : msg(std::move(msg)) { }

        BaseException() = default;

        virtual char const *what() const noexcept override {
            return msg.c_str();
        }

        void set_msg(const string &msg) {
            this->msg = msg;
        }
    };

    // 模版异常类
    class TemplateNotFoundException: public BaseException{
    public:
        explicit TemplateNotFoundException(const string& msg):BaseException(msg) {  }
        TemplateNotFoundException() { set_msg("template not found"); }
    };

    class TemplateParseException: public BaseException {
    public:
        explicit TemplateParseException(const string& msg): BaseException(msg) { }
        TemplateParseException() { set_msg("模板解析错误"); }
    };

    class FileException: public BaseException {
    public:
        explicit FileException(const string& msg): BaseException(msg) { }
        FileException() { set_msg("文件读写错误"); }
    };
}
#endif