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

        instruction(const std::string& name_, const std::vector<std::string>& operands_)
            : name(name_)
            , operands(operands_)
        {

        }

        std::string name;
        std::vector<std::string> operands;

        virtual void exec(execution_context& context, common::bitreader& reader) = 0;
    };

    using program = std::vector<instruction::ptr>;
}