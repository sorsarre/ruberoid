#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class ctxf: public instruction
    {
    public:
        static constexpr const std::string name = "ctxf";

        //----------------------------------------------------------------------
        ctxf(const std::vector<std::string>& args) = default;

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            context._context_stack.pop();
        }
    };
}