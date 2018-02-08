#pragma once
#include <memory>
#include <any>
#include "ruberoid/runtime/context.hpp"
#include "ruberoid/runtime/array.hpp"

namespace
{
    template<typename T>
    struct is_string:
        std::disjunction<
            std::is_same<std::string, T>,
            std::is_same<const char*, T>> {};

    template<typename T>
    constexpr const bool is_string_v = is_string<T>::value;
}

namespace rb::runtime
{
    class value
    {
    public:
        enum class kind {
            context,
            array,
            primitive,
            string,
            null
        };

        using ptr = std::shared_ptr<value>;

        //----------------------------------------------------------------------
        bool context() const
        {
            return (_kind == kind::context);
        }

        //----------------------------------------------------------------------
        bool array() const
        {
            return (_kind == kind::array);
        }

        //----------------------------------------------------------------------
        bool arithmetic() const
        {
            return (_kind == kind::primitive);
        }

        //----------------------------------------------------------------------
        bool string() const
        {
            return (_kind == kind::string);
        }

        //----------------------------------------------------------------------
        template<typename T>
        std::enable_if_t<std::is_scalar_v<T>> set(const T& actual)
        {
            _actual = actual;
            _kind = kind::primitive;
        }

        //----------------------------------------------------------------------
        template<typename T>
        std::enable_if_t<is_string_v<T>> set(const T& actual)
        {
            _actual = std::string(actual);
            _kind = kind::string;
        }

        //----------------------------------------------------------------------
        void set(context::ptr ctx)
        {
            _actual = ctx;
            _kind = kind::context;
        }

        //----------------------------------------------------------------------
        void set(array::ptr arr)
        {
            _actual = arr;
            _kind = kind::context;
        }

        //----------------------------------------------------------------------
        template<typename T>
        T& get()
        {
            return std::any_cast<T&>(_actual);
        }

        //----------------------------------------------------------------------
        template<typename T>
        const T& get() const
        {
            return std::any_cast<const T&>(_actual);
        }

        //----------------------------------------------------------------------
        std::any& get()
        {
            return _actual;
        }

        //----------------------------------------------------------------------
        const std::any& get() const
        {
            return _actual;
        }

        //----------------------------------------------------------------------
        operator bool() const
        {
            return (_kind != kind::null) && (_actual.has_value());
        }

    private:
        kind _kind;
        std::any _actual;
    };
}