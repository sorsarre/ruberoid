#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class slote: public instruction
    {
    public:
        static const std::string name() { return "slote"; }

        //----------------------------------------------------------------------
        slote(const std::vector<std::string>& args)
        {

        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            auto arr = context._slot_stack.top()->get<array::ptr>();
            auto new_slot = std::make_shared<value>();
            arr->add(new_slot);
            context._slot_stack.push(new_slot);
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