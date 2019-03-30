//
// Created by hhk on 18-12-17.
//

#ifndef SIMPLE_JSON_H
#define SIMPLE_JSON_H

#include "any.hpp"
#include "JSONBase.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONException.hpp"

#define KEYVALUE(key,value) {key, value}
#define LIST(...) {__VA_ARGS__}
#define OBJECT(...) {__VA_ARGS__}

static std::ostream &operator<<(std::ostream &os, EasyJson::any &rhs) {
#define CHECKTYPE(target_type) \
    if (rhs.type().hash_code() == typeid(target_type).hash_code()) { \
        os << EasyJson::any_cast<target_type>(rhs); \
        return os; \
    }

    CHECKTYPE(int);
    CHECKTYPE(float);
    CHECKTYPE(double);
    CHECKTYPE(std::string);
    CHECKTYPE(EasyJson::JSONArray);
    CHECKTYPE(EasyJson::JSONObject);
    CHECKTYPE(char*);
    CHECKTYPE(const char*);
    if (rhs.type().hash_code() == typeid(nullptr).hash_code())
        os << "null";
    CHECKTYPE(unsigned int);
    CHECKTYPE(long);
    CHECKTYPE(unsigned long);
    CHECKTYPE(short);

    return os;
#undef CHECKTYPE
}

#endif //SIMPLE_JSON_H
