//
// Created by hhk on 18-12-17.
//

#include "JSONObject.h"
#include "JSONArray.h"

namespace EasyJson {
    string JSONArray::__toString(int indent, size_t to_indent) {
        stringstream ss;
        string s;
        if (getSize() == 0) return "[]";
        ss << "[";
        string indent_str;
        if (indent >= 0) {
            ss << '\n';
            to_indent += indent;
            indent_str = string(static_cast<size_t>(to_indent), ' ');
        }
        static auto string_hash_code = typeid(string).hash_code();
        static auto const_pchar_hash_code = typeid(const char *).hash_code();
        static auto pchar_hash_code = typeid(char *).hash_code();
        static auto jsonobj_hash_code = typeid(JSONObject).hash_code();
        static auto jsonarr_hash_code = typeid(JSONArray).hash_code();
        static auto nullptr_hash_code = typeid(nullptr).hash_code();
        bool first = true;
        bool last_result = true;
        string tmp_str;
        size_t code;
        size_t cnt = 0;
        for (auto &item : data) {
            if (!first && last_result) {
                ss << ',';
                if (indent >= 0) ss << '\n';
                else ss << ' ';
            }
            if (indent >= 0)
                ss << indent_str;
            code = item.type().hash_code();
            last_result = true;
            if (code == string_hash_code) {
                tmp_str = item.get<string>();
                replace_str(tmp_str, "\"", "\\\"");
                ss << '"';
                ss << tmp_str;
                ss << '"';
            } else if (code == const_pchar_hash_code || code == pchar_hash_code) {
                try {
                    tmp_str = string(item.get<const char *>());
                } catch (...) {
                    tmp_str = string(item.get<char *>());
                }
                replace_str(tmp_str, "\"", "\\\"");
                ss << '"';
                ss << tmp_str;
                ss << '"';
            } else if (code == jsonobj_hash_code) {
                ss << any_cast<JSONObject>(item).__toString(indent, to_indent);
            } else if (code == jsonarr_hash_code) {
                ss << any_cast<JSONArray>(item).__toString(indent, to_indent);
            } else if (code == nullptr_hash_code) {
                ss << "null";
            }
            else
                last_result = output_field(ss, item);
            if(last_result) cnt++;
            first = false;
        }
        s = ss.str();
        if(cnt == 0) return "[]";
        if (!last_result) {
            s.erase(s.find_last_of(','));
        }
        if (indent >= 0) {
            to_indent -= indent;
            indent_str = string(static_cast<size_t>(to_indent), ' ');
            tmp_str = "\n" + indent_str + "]";
        } else
            tmp_str = "]";
        return s + tmp_str;
    }

    JSONArray::JSONArray(const initializer_list<JSONInitList> &a) {
        type = ARRAY;
        static auto hash_code1 = typeid(int).hash_code();
        static auto hash_code2 = typeid(float).hash_code();
        static auto hash_code3 = typeid(double).hash_code();
        static auto hash_code4 = typeid(string).hash_code();
        static auto hash_code5 = typeid(nullptr).hash_code();
        static auto hash_code6 = typeid(initializer_list<JSONInitList>).hash_code();
        static auto hash_code7 = typeid(JSONArray).hash_code();
        static auto hash_code8 = typeid(JSONObject).hash_code();
        for(auto& item: a){
            auto hash_code = item.type().hash_code();
            if(hash_code == hash_code1 ||
               hash_code == hash_code2 ||
               hash_code == hash_code3 ||
               hash_code == hash_code4 ||
               hash_code == hash_code5 ||
               hash_code == hash_code7 ||
               hash_code == hash_code8 )
                data.emplace_back(item.value);
            else if(hash_code == hash_code6) {
                auto value2 = any_cast<initializer_list<JSONInitList>>(item.value);
                if(checkInitListType(value2) == OBJECT) {
                    // 判定为object
                    data.emplace_back(JSONObject(value2));
                } else{
                    // 判定为array
                    data.emplace_back(JSONArray(value2));
                }
            }
        }
    }

    JSONArray &JSONArray::add(const initializer_list<JSONInitList> &s) {
        if(checkInitListType(s) == ARRAY)
            data.emplace_back(JSONArray(s));
        else
            data.emplace_back(JSONObject(s));
        return *this;
    }
}