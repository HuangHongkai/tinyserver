#ifndef SIMPLE_JSON_ANY_HPP
#define SIMPLE_JSON_ANY_HPP
#include <bits/stdc++.h>

namespace EasyJson {
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

    class Object {
    public:
            friend std::ostream& operator << (std::ostream& os, Object& other) {
                os << other.toString();
                return os;
            }

            virtual std::string toString() = 0;

            const std::type_info& type() const {
                return typeid(*this);
            }

            const size_t hashCode() const {
                return type().hash_code();
            };

            virtual bool operator == (Object& other) {
                return type() == other.type();
            }

            /**
             * 参考java.lang.Object
             * public final native void notify();
             *
             * Wakes up a single thread that is waiting on this object's
             * monitor. If any threads are waiting on this object, one of them
             * is chosen to be awakened. The choice is arbitrary and occurs at
             * the discretion of the implementation. A thread waits on an object's
             * monitor by calling one of the wait methods
             */
            virtual void notify() final{

            }

            /**
             * 参考java.lang.Object
             * public final native void notifyAll();
             * Wakes up all threads that are waiting on this object's monitor. A
             * thread waits on an object's monitor by calling one of the
             * wait methods.
             */
            virtual void notifyAll() final{

            }

            /**
             * 参考java.lang.Object
             * public final native void wait(long timeout) throws InterruptedException;
             * Causes the current thread to wait until either another thread invokes the
             * {@link java.lang.Object#notify()} method or the
             * {@link java.lang.Object#notifyAll()} method for this object, or a
             * specified amount of time has elapsed.
             * <p>
             * The current thread must own this object's monitor.
             */
            virtual void wait(long timeout) final {

            }
        };
}

#endif // SIMPLE_JSON_ANY_HPP