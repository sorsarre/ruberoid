#pragma once
#include <vector>
#include <memory>
#include <stack>
#include "ruberoid/runtime/context.hpp"
#include "ruberoid/runtime/value.hpp"

namespace rb::runtime
{
    struct execution_context
    {
        using context_stack = std::stack<context::ptr>;
        using slot_stack = std::stack<value::ptr>;
        using frame_stack = std::stack<size_t>;

        size_t _position;
        context_stack _context_stack;
        slot_stack _slot_stack;
        frame_stack _frame_stack;
    };
}