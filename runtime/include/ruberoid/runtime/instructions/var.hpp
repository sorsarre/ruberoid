#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class var: public instruction
    {
    public:
        static const std::string name() { return "var"; }

        //----------------------------------------------------------------------
        var(const std::vector<std::string>& args)
        {
            _var_name = args.front();
        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            auto new_slot = std::make_shared<value>();
            context._context_stack.current_scope()->set_field(_var_name, new_slot);
        }

        //----------------------------------------------------------------------
        std::string to_string() override
        {
            std::ostringstream oss;
            oss << name() << " " << _var_name;
            return oss.str();
        }

    private:
        std::string _var_name;
    };
}