//
// Created by hhk on 18-12-17.
//

#include "JSONBase.h"
#include "JSONArray.h"
#include "JSONObject.h"


namespace EasyJson {
    bool JSONBase::output_field(ostream &os, const any &rhs) {
#define CHECKTYPE(target_type) \
    if (rhs.type().hash_code() == typeid(target_type).hash_code()) { \
        os << any_cast<target_type>(rhs); \
        return true; \
    }

        CHECKTYPE(int);
        CHECKTYPE(float);
        CHECKTYPE(double);
        //CHECKTYPE(JSONArray);
        //CHECKTYPE(JSONObject);
        //CHECKTYPE(char*);
        //CHECKTYPE(const char*);
        //CHECKTYPE(string);
#undef CHECKTYPE
        return false;
    }

    JSONArray *JSONBase::parse_arr(string &str) {
        trim(str);
        size_t i = 1, j;
        size_t len = str.length();
        auto *arr_ptr = new JSONArray();
        JSONArray &arr = *arr_ptr;
        string tmp;
        if (!checkJson(str) && str[0] == '[')
            throwException(JSONParseException, "not ejson arr");
        while (i < len) {
            if (str[i] == ' ' || str[i] == '\r' || str[i] == '\n') {
                i++;
                continue;
            }
            j = str.find(',', i);
            if (j == string::npos)
                j = len - 1;
            if (str[i] == '"') {
                j = i;
                for (j++; j < len; j++) {
                    if (str[j] == '"')
                        break;
                    else if (str[j] == '\\' && j + 1 < len && str[j + 1] == '"')
                        j++;
                }
                j = str.find(',', j);
                if (j == string::npos)
                    j = len - 1;
                tmp = str.substr(i, j - i);
                tmp = parseString(tmp);
                arr.add(tmp);
            } else if (str[i] == '{') {
                j = findNext(str, i, '}');
                if (i == j)
                    throwException(JSONParseException);
                tmp = str.substr(i, j - i + 1);
                JSONObject *obj2 = parse_obj(tmp);
                arr.add(*obj2);
                delete obj2;
                j = str.find(',', j);
                if (j == string::npos)
                    j = len - 1;
            } else if (str[i] == '[') {
                j = findNext(str, i, ']');
                if (i == j || i == len - 1)
                    throwException(JSONParseException);
                tmp = str.substr(i, j - i + 1);
                JSONArray *arr2 = parse_arr(tmp);
                arr.add(*arr2);
                delete arr2;
                j = str.find(',', j);
                if (j == string::npos)
                    j = len - 1;
            } else if (str[i] == 'n') {
                tmp = str.substr(i, j - i);
                arr.add(parseNull(tmp));
            } else if (isdigit(str[i])) {
                bool isDouble = false;
                size_t k;
                for (k = i; k < j; k++) {
                    if (str[k] == '.') {
                        isDouble = true;
                        break;
                    }
                }
                tmp = str.substr(i, j - i);
                if (isDouble)
                    arr.add(parseDouble(tmp));
                else
                    arr.add(parseInt(tmp));
            } else {
                if (str[i] == ']' && arr.getSize() == 0 && i == len - 1)
                    return arr_ptr;
                throwException(JSONParseException);
            }
            i = j + 1;
        }
        return arr_ptr;
    }

    JSONObject *JSONBase::parse_obj(string &str) {
        trim(str);
        auto *obj_ptr = new JSONObject();
        JSONObject &obj = *obj_ptr;
        size_t i = 1, j;
        size_t len = str.length();
        string tmp, key, value;
        if (!checkJson(str) && str[0] == '{')
            throwException(JSONParseException, "not ejson");
        while (i < len) {
            if (str[i] == ' ' || str[i] == '\n' || str[i] == '\r') {
                i++;
                continue;
            }
            /* 处理key */
            if (str[i] != '"') {
                if (str[i] == '}' && obj.getSize() == 0 && i == len - 1)
                    return obj_ptr;
                throwException(JSONParseException, );;
            }
            j = i;
            for (j++; j < len; j++) {
                if (str[j] == '"')
                    break;
                else if (str[j] == '\\' && j + 1 < len && str[j + 1] == '"')
                    i++;
            }
            tmp = str.substr(i, j - i + 1);
            key = parseString(tmp);
            for (j++; j < len && str[j] == ' '; j++);
            if (str[j] != ':') throwException(JSONParseException);

            /* 处理value */
            for (i = j + 1; i < len && str[i] == ' '; i++) continue;
            if (i >= len) throwException(JSONParseException);
            j = str.find(',', i);
            if (j == string::npos)
                j = len - 1;
            if (str[i] == '"') {
                j = i;
                for (j++; j < len; j++) {
                    if (str[j] == '"')
                        break;
                    else if (str[j] == '\\' && j + 1 < len && str[j + 1] == '"')
                        j++;
                }
                size_t j2 = str.find(',', j);
                if (j2 == string::npos)
                    j2 = len - 1;

                j = j2;
                tmp = str.substr(i, j - i);
                tmp = parseString(tmp);
                obj.add(key, tmp);
            } else if (str[i] == '{') {
                j = findNext(str, i, '}');
                if (i == j)
                    throwException(JSONParseException);
                tmp = str.substr(i, j - i + 1);
                JSONObject *obj2 = parse_obj(tmp);
                obj.add(key, *obj2);
                delete obj2;
                j = str.find(',', j);
                if (j == string::npos)
                    j = len - 1;
            } else if (str[i] == '[') {
                j = findNext(str, i, ']');
                if (i == j || i == len - 1)
                    throwException(JSONParseException);
                tmp = str.substr(i, j - i + 1);
                JSONArray *arr2 = parse_arr(tmp);
                obj.add(key, *arr2);
                delete arr2;
                j = str.find(',', j);
                if (j == string::npos)
                    j = len - 1;
            } else if (str[i] == 'n') {
                tmp = str.substr(i, j - i);
                obj.add(key, parseNull(tmp));
            } else if (isdigit(str[i])) {
                bool isDouble = false;
                size_t k;
                for (k = i; k < j; k++) {
                    if (str[k] == '.') {
                        isDouble = true;
                        break;
                    }
                }
                tmp = str.substr(i, j - i);
                if (isDouble)
                    obj.add(key, parseDouble(tmp));
                else
                    obj.add(key, parseInt(tmp));
            } else {
                throwException(JSONParseException, );;
            }

            i = j + 1;
        }
        return obj_ptr;
    }

    JSONBase *JSONBase::parse(string &str) {
        trim(str);
        if (str[0] == '{')
            return parse_obj(str);
        else if (str[0] == '[')
            return parse_arr(str);
        throwException(JSONParseException, );;
    }

    double JSONBase::parseDouble(string &str) {
        return strtod(str.c_str(), nullptr);
    }

    void* JSONBase::parseNull(string &str) {
        if (str.find("null") == string::npos)
            throwException(JSONParseException, );;
        return nullptr;
    }

    string JSONBase::parseString(string &str) {
        trim_right(str);
        size_t len = str.length();
        if (str[len - 1] != '"' || str[0] != '"')
            throwException(JSONParseException, );
        replace_str(str, "\\\"", "\"");
        return str.substr(1, len - 2);
    }

    int JSONBase::parseInt(string &str) {
        return static_cast<int>(strtol(str.c_str(), nullptr, 0));
    }

    size_t JSONBase::findNext(string &str, size_t start, char rev) {
        char key = str[start];
        size_t i = start + 1;
        size_t cnt = 1;
        size_t len = str.size();
        for (; i < len; i++) {
            if (str[i] == rev) {
                cnt--;
                if (cnt == 0)
                    break;
            } else if (str[i] == key)
                cnt++;
            else if (str[i] == '"') {
                for (i++; i < len; i++) {
                    if (str[i] == '"')
                        break;
                    else if (str[i] == '\\' && i + 1 < len && str[i + 1] == '"')
                        i++;
                }
            }
        }
        if (cnt != 0)
            return start;
        return i;
    }

    bool JSONBase::checkJson(string &str) {
        size_t l1, l2, r1, r2;
        l1 = l2 = r1 = r2 = 0; // l1: [ 数量 ; r1: ] 数量 ; l2: { 数量 ; r2: }数量
        size_t len = str.length();
        size_t i;
        for (i = 0; i < len; i++) {
            char ch = str[i];
            if (ch == '[')
                l1++;
            else if (ch == ']')
                r1++;
            else if (ch == '{')
                l2++;
            else if (ch == '}')
                r2++;
            else if (ch == '"') { // 忽略字符串里面的信息
                for (i++; i < len; i++) {
                    if (str[i] == '"')
                        break;
                    else if (str[i] == '\\' && i + 1 < len && str[i + 1] == '"')
                        i++;
                }
            }
        }
        return l1 == r1 && l2 == r2;
    }

    JSONBase::JSONType JSONBase::checkInitListType(const initializer_list<JSONInitList> &s) {
        static auto hash_code4 = typeid(string).hash_code();
        static auto hash_code6 = typeid(initializer_list<JSONInitList>).hash_code();
        for(auto& item: s) {
            if(item.type().hash_code() != hash_code6)
                return ARRAY;
            auto var = any_cast<initializer_list<JSONInitList>>(item.value);
            if(var.size() != 2 || var.begin()->value.type().hash_code() != hash_code4) {
                return ARRAY;
            }
        }
        return OBJECT;
    }
}
