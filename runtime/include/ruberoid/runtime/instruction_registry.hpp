#pragma once
#include <unordered_map>
#include <functional>
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime
{
    class instruction_registry
    {
    public:
        using instruction_args = std::vector<std::string>;
        using instruction_factory = std::function<instruction::ptr(const instruction_args&)>;

        //----------------------------------------------------------------------
        void register_instruction_explicit(const std::string& name, instruction_factory factory)
        {
            _registry.emplace(name, factory);
        }

        //----------------------------------------------------------------------
        template<typename Instr>
        size_t register_instruction()
        {
            _registry.emplace(Instr::name(), [] (const instruction_args& args) -> auto {
                return std::make_shared<Instr>(args);
            });
            return _registry.size();
        }

        //----------------------------------------------------------------------
        instruction::ptr parse(const std::string& name, const instruction_args& args)
        {
            if (!_registry.count(name)) {
                throw std::runtime_error("Illegal instruction!");
            }

            return _registry[name](args);
        }

        //----------------------------------------------------------------------
        static instruction_registry& instance()
        {
            static instruction_registry _instance;
            return _instance;
        }

    private:
        instruction_registry()
        {
            register_instructions();
        }

        instruction_registry(const instruction_registry&) = delete;
        instruction_registry(instruction_registry&&) = delete;
        instruction_registry& operator=(const instruction_registry&) = delete;

        void register_instructions();

        std::unordered_map<std::string, instruction_factory> _registry;
    };
}