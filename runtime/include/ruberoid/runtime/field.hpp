#pragma once
#include <memory>

namespace rb::runtime
{
    class value;

    class field
    {
    public:
        using ptr = std::shared_ptr<field>;
        using value_ptr = std::shared_ptr<value>;

        //----------------------------------------------------------------------
        explicit field(const std::string& name): _name(name)
        {

        }

        //----------------------------------------------------------------------
        void set_value(value_ptr value)
        {
            _value = value;
        }

        //----------------------------------------------------------------------
        value_ptr get_value()
        {
            return _value;
        }

        //----------------------------------------------------------------------
        operator bool() const
        {
            return (_value != nullptr);
        }

        //----------------------------------------------------------------------
        const std::string& name() const
        {
            return _name;
        }

    private:
        std::string _name;
        value_ptr _value;
    };
}