//
// Created by hhk on 18-12-17.
//

#include "JSONObject.h"
#include "JSONArray.h"

namespace EasyJson {

    string JSONObject::__toString(int indent, size_t to_indent) {
        stringstream ss;
        string s;
        if(getSize() == 0) return "[]";
        ss << '{';
        string indent_str;
        if(indent >= 0) {
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
        for (auto &item : data) {
            if (!first && last_result){
                ss << ',';
                if(indent >= 0) ss << '\n';
                else ss << ' ';
            }
            if(indent>=0)
                ss << indent_str;
            ss << '"';
            tmp_str = item.first;
            replace_str(tmp_str, "\"", "\\\"");
            ss << tmp_str;
            ss << "\": ";
            code = item.second.type().hash_code();
            last_result = true;
            if (code == string_hash_code) {
                tmp_str = item.second.get<string>();
                replace_str(tmp_str, "\"", "\\\"");
                ss << '"';
                ss << tmp_str;
                ss << '"';
            } else if (code == const_pchar_hash_code || code == pchar_hash_code) {
                try {
                    tmp_str = string(item.second.get<const char *>());
                } catch (...) {
                    tmp_str = string(item.second.get<char *>());
                }
                replace_str(tmp_str, "\"", "\\\"");
                ss << '"';
                ss << tmp_str;
                ss << '"';
            } else if (code == jsonobj_hash_code) {
                ss << any_cast<JSONObject>(item.second).__toString(indent, to_indent);
            } else if (code == jsonarr_hash_code) {
                ss << any_cast<JSONArray>(item.second).__toString(indent, to_indent);
            } else if (code == nullptr_hash_code) {
                ss << "null";
            }
            else
                last_result = output_field(ss, item.second);
            first = false;
        }
        s = ss.str();
        if(!last_result) {
            s.erase(s.find_last_of(','));
        }
        if(indent >= 0) {
            to_indent -= indent;
            indent_str = string(static_cast<size_t>(to_indent), ' ');
            tmp_str = "\n"+indent_str+"}";
        } else
            tmp_str = "}";
        return s+tmp_str;
    }

    JSONObject::JSONObject(const initializer_list<JSONInitList> &s){
        type = OBJECT;
        static auto hash_code1 = typeid(int).hash_code();
        static auto hash_code2 = typeid(float).hash_code();
        static auto hash_code3 = typeid(double).hash_code();
        static auto hash_code4 = typeid(string).hash_code();
        static auto hash_code5 = typeid(nullptr).hash_code();
        static auto hash_code6 = typeid(initializer_list<JSONInitList>).hash_code();
        static auto hash_code7 = typeid(JSONArray).hash_code();
        static auto hash_code8 = typeid(JSONObject).hash_code();
        for(auto& item: s){
            if(item.type().hash_code() != hash_code6)
                continue;
            auto var = any_cast<initializer_list<JSONInitList>>(item.value);
            if(var.size() != 2 || var.begin()->value.type().hash_code() != hash_code4) {
                std::cerr << "parse err " << std::endl;
                continue;
            }
            auto hash_code = (var.begin()+1)->value.type().hash_code();
            auto key = any_cast<string>(var.begin()->value);
            auto value = (var.begin()+1)->value;
            if(hash_code == hash_code1 ||
               hash_code == hash_code2 ||
               hash_code == hash_code3 ||
               hash_code == hash_code4 ||
               hash_code == hash_code5 ||
               hash_code == hash_code7 ||
               hash_code == hash_code8 )
                data[key] = value;
            else if(hash_code == hash_code6) {
                auto value2 = any_cast<initializer_list<JSONInitList>>(value);
                if(checkInitListType(value2) == OBJECT) {
                    // 判定为object
                    data[key] = JSONObject(value2);
                } else {
                    // 判定为array
                    data[key] = JSONArray(value2);
                }
            }
        }
    }

    JSONObject& JSONObject::add(const string &key, const initializer_list<JSONInitList> &a) {
        if(checkInitListType(a) == ARRAY)
            data[key] = JSONArray(a);
        else
            data[key] = JSONObject(a);
        return *this;
    }
}