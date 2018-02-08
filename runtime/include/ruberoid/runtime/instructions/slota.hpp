#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class slota: public instruction
    {
    public:
        static const std::string name() { return "slota"; }

        //----------------------------------------------------------------------
        slota(const std::vector<std::string>& args)
        {

        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            auto slot = context._slot_stack.top();
            auto new_array = std::make_shared<array>();
            slot->set(new_array);
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