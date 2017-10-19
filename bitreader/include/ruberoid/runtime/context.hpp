#pragma once
#include <memory>
#include <unordered_map>
#include "ruberoid/runtime/field.hpp"

namespace rb::runtime
{
    class value;

    class context
    {
    public:
        using ptr = std::shared_ptr<context>;
        using value_ptr = std::shared_ptr<value>;

        context() = delete;
        context(const context&) = delete;
        context(context&&) = default;

        //----------------------------------------------------------------------
        context(const std::string& type): _type(type)
        {

        }

        //----------------------------------------------------------------------
        field::ptr get_field(const std::string& name)
        {
            return (_fields.count(name) > 0) ? _fields[name] : nullptr;
        }

        //----------------------------------------------------------------------
        void set_field(const std::string& name, value_ptr val)
        {
            auto new_field = std::make_shared<field>(name);
            new_field->set_value(val);
            _fields[name] = new_field;
        }

        //----------------------------------------------------------------------
        bool has(const std::string& name)
        {
            return (_fields.count(name) > 0);
        }

        //----------------------------------------------------------------------
        value_ptr get(const std::string& name)
        {
            auto f = get_field(name);
            if (!f) {
                throw std::runtime_error("Field " + name + " is missing from the context");
            }

            return f->get_value();
        }

        //----------------------------------------------------------------------
        const std::string& type() const
        {
            return _type;
        }
    private:
        std::string _type;
        std::unordered_map<std::string, field::ptr> _fields;
    };
}