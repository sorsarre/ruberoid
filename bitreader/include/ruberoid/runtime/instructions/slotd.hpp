#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class slotd: public instruction
    {
    public:
        static constexpr const std::string name = "slotd";

        //----------------------------------------------------------------------
        slotd(const std::vector<std::string>& args) = default;

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            context._slot_stack.pop();
        }
    };
}