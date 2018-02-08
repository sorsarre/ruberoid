#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class slotd: public instruction
    {
    public:
        static const std::string name() { return "slotd"; }

        //----------------------------------------------------------------------
        slotd(const std::vector<std::string>& args)
        {

        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            context._slot_stack.pop();
        }

        //----------------------------------------------------------------------
        std::string to_string() override
        {
            std::ostringstream oss;
            oss << name();
            return oss.str();
        }
    };
}