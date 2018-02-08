#pragma once
#include "ruberoid/runtime/instruction.hpp"
#include "ruberoid/runtime/instruction_registry.hpp"

namespace rb::runtime
{
    using raw_program = std::vector< std::vector<std::string> >;

    class program_parser
    {
    public:
        static program parse(const raw_program& raw)
        {
            program ret;
            auto& registry = instruction_registry::instance();
            std::transform(
                raw.begin(),
                raw.end(),
                std::back_inserter(ret),
                [&](const std::vector<std::string>& args) -> auto {
                    auto args_copy = args;
                    auto name = args.front();
                    args_copy.erase(args_copy.begin());
                    return registry.parse(name, args_copy);
                }
            );
            return ret;
        }
    };
}