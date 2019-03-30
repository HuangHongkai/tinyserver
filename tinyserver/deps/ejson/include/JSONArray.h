//
// Created by hhk on 18-12-17.
//

#ifndef JSONARRAY_H
#define JSONARRAY_H
#include <bits/stdc++.h>
#include "JSONBase.h"

namespace EasyJson {
    using std::stringstream;
    using std::vector;
    using std::initializer_list;
    class JSONBase;

    class JSONArray : public JSONBase {
        vector <any> data;

    public:
        JSONArray() { type = ARRAY; }

        JSONArray(const initializer_list<JSONInitList>& a);

        explicit JSONArray(vector<any>& s):data(s) { type = ARRAY; }

        explicit JSONArray(size_t n){ type = ARRAY; data.resize(n); }

        ~JSONArray() override = default;


#define JSONArray_add_gen(type) \
    JSONArray& add(type a) { \
        data.emplace_back(a); \
        return *this; \
    }

        JSONArray_add_gen(int);

        JSONArray_add_gen(float);

        JSONArray_add_gen(double);

        JSONArray_add_gen(const string&);

        JSONArray_add_gen(JSONArray &);

        JSONArray_add_gen(JSONObject &);

        JSONArray& add(void* ptr) {
            if(ptr == nullptr)
                data.emplace_back(nullptr);
            return *this;
        }

        any& operator [](size_t index) {
            size_t size = getSize();
            if(index >= size) {
                for(size_t i=size;i < index+1;i++)
                    data.emplace_back(nullptr);
            }
            return data[index];
        }

        size_t getSize() const override { return data.size(); }

        JSONArray& add(vector <any> &a) {
            for (auto &item: a)
                data.emplace_back(item);
            return *this;
        }

        JSONArray& add(const initializer_list<JSONInitList>& s);

        template<typename T>
        T get(size_t key) { return any_cast<T>(data[key]); }

        vector <any> &getData() { return data; }

        string __toString(int indent, size_t to_indent) override;
    };
}
#endif //JSONARRAY_H
