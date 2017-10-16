#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class call: public instruction
    {
    public:
        static constexpr const std::string name = "call";

        //----------------------------------------------------------------------
        call(const std::vector<std::string>& args)
        {
            _label = args.front();
        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            // TODO: No label reference info available in the program currently... :(
            size_t jump_to = _position; // FIXME: Jump to function location
            context._frame_stack.push(context._position);
            context._position = jump_to;
        }

    private:
        std::string _label;
    };
}