#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class ret: public instruction
    {
    public:
        static constexpr const std::string name = "ret";

        //----------------------------------------------------------------------
        ret(const std::vector<std::string>& args) = default;

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            context._position = _frame_stack.top();
            context._frame_stack.pop();
        }
    };
}