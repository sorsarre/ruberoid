#pragma once
#include <vector>
#include <string>
#include "ruberoid/bitreader/bitreader.hpp"
#include "ruberoid/runtime/execution_context.hpp"

namespace rb::runtime
{
    class instruction
    {
    public:
        using ptr = std::shared_ptr<instruction>;

        virtual void exec(execution_context& context, common::bitreader& reader) = 0;
        virtual ~instruction() = default;
    };

    using program = std::vector<instruction::ptr>;
}