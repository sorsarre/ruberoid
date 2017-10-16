#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class slotf: public instruction
    {
    public:
        static constexpr const std::string name = "slotf";

        //----------------------------------------------------------------------
        slotf(const std::vector<std::string>& args)
        {
            _field_name = args.front();
        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            auto new_slot = std::make_shared<value>();
            context._context_stack.top()->field(_field_name)->set(new_slot);
            context._slot_stack.push(new_slot);
        }

    private:
        std::string _field_name;
    };
}