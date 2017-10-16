#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class ctxc: public instruction
    {
    public:
        static constexpr const std::string name = "ctxc";

        //----------------------------------------------------------------------
        ctxc(const std::vector<std::string>& args)
        {
            _ctxid = args.front();
        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            auto new_ctx = std::make_shared<context>(_ctxid);
            context._slot_stack.top()->set(new_ctx);
            context._context_stack.push(new_ctx);
        }

    private:
        std::string _ctxid;
    };
}