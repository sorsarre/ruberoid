#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class ctxf: public instruction
    {
    public:
        static const std::string name() { return "ctxf"; }

        //----------------------------------------------------------------------
        ctxf(const std::vector<std::string>& args)
        {

        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            context._context_stack.pop_context();
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