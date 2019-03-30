//
// Created by hhk on 18-12-17.
//
#ifndef JSONBASE_H
#define JSONBASE_H
#include <bits/stdc++.h>
#include "any.hpp"
#include "StringUtils.h"
#include "JSONException.hpp"

namespace EasyJson {
    using std::ostream;
    using std::string;
    using std::initializer_list;
    class JSONArray;
    class JSONObject;

    class JSONInitList {
    public:
        any value;
        template<typename T>
        JSONInitList(const T& value) {
            this->value = value;
        }

        JSONInitList(const initializer_list<JSONInitList>& value) {
            this->value = value;
        }

        const std::type_info& type() const {
            return this->value.type();
        }

    };

    // TODO 支持文件读写
    class JSONBase {
    public:
        virtual ~JSONBase() = default;

        string toString(int indent = -1) { return __toString(indent, 0); };

        enum JSONType {
            UNKNOWN,
            ARRAY,
            OBJECT,
        } type = UNKNOWN;

        bool EasyJson() { return type == ARRAY; }

        bool isJSONOBject() { return type == OBJECT; }

        static bool output_field(ostream &os, const any &rhs);

        friend ostream &operator<<(ostream &os, const JSONBase &rhs) {
            os << const_cast<JSONBase &>(rhs).toString(); // const_cast 转为非const引用
            return os;
        }

        static JSONArray *parse_arr(string &str);

        static JSONObject *parse_obj(string &str);

        static JSONBase *parse(string &str);

        virtual size_t getSize() const = 0;

    protected:
        virtual string __toString(int indent, size_t to_indent) = 0;
        static JSONBase::JSONType checkInitListType(const initializer_list<JSONInitList>& s);

    private:

        static int parseInt(string &str);

        static double parseDouble(string &str);

        static void* parseNull(string &str);

        static string parseString(string &str);

        static size_t findNext(string &str, size_t start, char rev);

        static bool checkJson(string &str);
    };
}
#endif // JSONBASE_HPP