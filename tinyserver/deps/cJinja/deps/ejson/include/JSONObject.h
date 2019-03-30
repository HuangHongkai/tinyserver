//
// Created by hhk on 18-12-17.
//

#ifndef JSONOBJECT_HPP
#define JSONOBJECT_HPP

#include <bits/stdc++.h>
#include "JSONBase.h"
#include "JSONArray.h"
#include "StringUtils.h"

namespace EasyJson {
    using std::map;
    using std::initializer_list;
    using std::vector;
    using std::stringstream;
    using std::pair;

    class JSONObject : public JSONBase {
        map<string, any> data;
    public:
        JSONObject() { type = OBJECT; }

        JSONObject(const initializer_list<JSONInitList> &s);

        template<typename T>
        explicit JSONObject(map<string, T> &data) {
            type = OBJECT;
            for(auto& item: data)
                this->data[item.first] = item.second;
        }

        ~JSONObject() override = default;

#define JSONObject_add_gen(type) \
    JSONObject& add(const string& key, type value) { \
        data[key] = value; \
        return *this; \
    }

        JSONObject_add_gen(JSONObject &);

        JSONObject_add_gen(JSONArray &);

        JSONObject_add_gen(int);

        JSONObject_add_gen(float);

        JSONObject_add_gen(double);

        JSONObject_add_gen(const string&);

        JSONObject& add(const string& key, void* ptr) {
            if(ptr == nullptr)
                data[key] = nullptr;
            return *this;
        }

        template<typename T>
        JSONObject& add(map<string, T> &s) {
            map<string, any> a;
            for(auto& item: s)
                a[item.first] = item.second;
            data.insert(a.begin(), a.end());
            return *this;
        }

        template<typename T>
        JSONObject& add(const string &key, map<string, T> &s) {
            data[key] = JSONObject(s);
        }

        JSONObject& add(const string &key, const initializer_list<JSONInitList> &a);

        map<string, any> &getData() {
            return data;
        }

        any& operator[](const string& key) {
            return data[key];
        }

        template<typename T>
        T get(const string &key) {
            auto it = data.find(key);
            if(it == data.end())
                return T();
            return any_cast<T>(it->second);
        }

        size_t getSize() const override { return data.size(); }

        string __toString(int indent, size_t to_indent) override;
    };
}
#endif
