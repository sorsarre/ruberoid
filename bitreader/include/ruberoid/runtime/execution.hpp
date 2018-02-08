#pragma once
#include "ruberoid/runtime/instruction.hpp"
#include "ruberoid/runtime/execution_context.hpp"
#include "ruberoid/bitreader/bitreader.hpp"

#include <iostream>

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
            std::cout << "[INSTR] " << instr->to_string() << std::endl;
            instr->exec(context, reader);
        }

        //----------------------------------------------------------------------
        void exec(const program& prog, common::bitreader& reader, execution_context& ctx)
        {
            auto default_ctx = std::make_shared<context>("__structure");
            ctx._context_stack.push_context(default_ctx);

            auto new_slot = std::make_shared<value>();
            ctx._context_stack.current_context()->set_field("__contents", new_slot);
            ctx._slot_stack.push(new_slot);

            while (ctx._position < prog.size()) {
                exec_next(prog, ctx, reader);
            }

            ctx._slot_stack.pop();
        }
    };
}