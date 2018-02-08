#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class ctxc: public instruction
    {
    public:
        static const std::string name() { return "ctxc"; }

        //----------------------------------------------------------------------
        ctxc(const std::vector<std::string>& args)
        {
            _ctxid = args.front();
        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& ctx, common::bitreader& reader)
        {
            // Create new context and push it onto the runtime environment
            auto new_ctx = std::make_shared<context>(_ctxid);
            ctx._context_stack.push_context(new_ctx);

            // Fill the current slot with the newly created context
            ctx._slot_stack.top()->set(new_ctx);

            // Add local scope in front of the context
            auto new_scope = std::make_shared<context>("");
            ctx._context_stack.push_scope(new_scope);
        }

        //----------------------------------------------------------------------
        std::string to_string() override
        {
            std::ostringstream oss;
            oss << name() << " " << _ctxid;
            return oss.str();
        }

    private:
        std::string _ctxid;
    };
}