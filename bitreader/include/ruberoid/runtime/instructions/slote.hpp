#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class slote: public instruction
    {
    public:
        static constexpr const std::string name = "slote";

        //----------------------------------------------------------------------
        slote(const std::vector<std::string>& args) = default;

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            auto new_slot = std::make_shared<value>();
            auto arr = context._slot_stack.top()->get<array::ptr>();
            new_slot->set(new_array);
            arr->add(new_slot);
            context._context_stack.top()->field(_field_name)->set(new_slot);
            context._slot_stack.push(new_slot);
        }
    };
}