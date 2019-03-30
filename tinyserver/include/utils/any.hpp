#ifndef WEBSERVER_ANY_HPP
#define WEBSERVER_ANY_HPP
#include <bits/stdc++.h>

namespace WebServer {
    /**
     * 支持任意 c++ 类型
     */
    class any {
    public: // structors

        any()
                : content(0) {
        }

        any(const char* value){ // support const char*
            content = new holder<std::string>(std::string(value));
        }

        template<typename ValueType>
        any(const ValueType &value)
                : content(new holder<ValueType>(value)) {
        }

        any(const any &other)
                : content(other.content ? other.content->clone() : 0) {
        }

        ~any() {
            delete content;
        }

    public: // modifiers

        any &swap(any &rhs) {
            std::swap(content, rhs.content);
            return *this;
        }

        template<typename ValueType>
        any &operator=(const ValueType &rhs) {
            any(rhs).swap(*this);
            return *this;
        }

        any &operator=(any rhs) {
            rhs.swap(*this);
            return *this;
        }

    public: // queries

        bool empty() const {
            return !content;
        }

        const std::type_info &type() const {
            return content ? content->type() : typeid(void);
        }

        template<class C>
        const C &get() const {
            const holder <C> &v = dynamic_cast<const holder <C> &>(*content);
            return v.held;
        }

    public: // types (public so any_cast can be non-friend)

        class placeholder {
        public: // structors

            virtual ~placeholder() {
            }

        public: // queries

            virtual const std::type_info &type() const = 0;

            virtual placeholder *clone() const = 0;

        };

        template<typename ValueType>
        class holder : public placeholder {
        public: // structors

            holder(const ValueType &value)
                    : held(value) {
            }

        public: // queries

            virtual const std::type_info &type() const {
                return typeid(ValueType);
            }

            virtual placeholder *clone() const {
                return new holder(held);
            }

        public: // representation

            ValueType held;

        private: // intentionally left unimplemented
            holder &operator=(const holder &);
        };

    public: // representation (public so any_cast can be non-friend)

        placeholder *content;
    };

    template<typename ValueType>
    ValueType any_cast(const any &operand) {
        assert(operand.type() == typeid(ValueType));
        return static_cast<any::holder<ValueType> *>(operand.content)->held;
    }
}

#endif // WEBSERVER_ANY_HPP