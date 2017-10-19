#pragma once
#include <vector>
#include <memory>
#include <stack>
#include <map>
#include <string>
#include "ruberoid/runtime/context.hpp"
#include "ruberoid/runtime/value.hpp"
#include "ruberoid/runtime/scope_stack.hpp"

namespace rb::runtime
{
    struct execution_context
    {
        using context_stack = scope_stack;
        using slot_stack = std::stack<value::ptr>;
        using frame_stack = std::stack<size_t>;
        using label_map = std::map<std::string, size_t>;

        size_t label(const std::string& name)
        {
            auto iter = _label_map.find(name);
            assert(iter != _label_map.end());

            return iter->second;
        }

        size_t _position;
        context_stack _context_stack;
        slot_stack _slot_stack;
        frame_stack _frame_stack;
        label_map _label_map;
    };
}