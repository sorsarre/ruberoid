#pragma once
#include <memory>
#include <vector>
#include <any>

namespace rb::runtime
{
    class value;

    class array
    {
    public:
        using ptr = std::shared_ptr<array>;
        using value_ptr = std::shared_ptr<value>;

        array() = default;

        //----------------------------------------------------------------------
        size_t size() const
        {
            return _data.size();
        }

        //----------------------------------------------------------------------
        value_ptr& operator[](size_t index)
        {
            return _data.at(index);
        }

        //----------------------------------------------------------------------
        const value_ptr& operator[](size_t index) const
        {
            return _data.at(index);
        }

        //----------------------------------------------------------------------
        bool empty() const
        {
            return _data.empty();
        }

        //----------------------------------------------------------------------
        void add(value_ptr value)
        {
            _data.push_back(value);
        }

    private:
        std::vector<value_ptr> _data;
    };
}