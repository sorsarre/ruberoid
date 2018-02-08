#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class ret: public instruction
    {
    public:
        static const std::string name() { return "ret"; }

        //----------------------------------------------------------------------
        ret(const std::vector<std::string>& args)
        {

        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            context._position = context._frame_stack.top();
            context._frame_stack.pop();
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