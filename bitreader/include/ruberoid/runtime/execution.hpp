#pragma once
#include "ruberoid/runtime/instruction.hpp"
#include "ruberoid/runtime/execution_context.hpp"
#include "ruberoid/bitreader/bitreader.hpp"

namespace rb::runtime
{
    class execution
    {
    public:
        //----------------------------------------------------------------------
        void exec_next(
            const program& prog,
            execution_context& context,
            common::bitreader& reader)
        {
            const auto& instr = prog.at(context._position);
            ++context._position;
            instr->exec(context, reader);
        }

        //----------------------------------------------------------------------
        value::ptr exec(const program& prog, common::bitreader& reader)
        {
            execution_context ctx;
            while (ctx._position < prog.size()) {
                exec_next(prog, ctx, reader);
            }

            return ctx._context_stack.top()->get("__structure");
        }
    };
}