#pragma once
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    template<typename T>
    inline constexpr size_t bit_size = sizeof(T)*8;

    template<typename T>
    inline constexpr const char* bit_string()
    {
        switch(bit_size<T>) {
        case 8:
            return "8";

        case 16:
            return "16";

        case 32:
            return "32";

        case 64:
            return "64";
        }
    }

    // FIXME: Incomplete

    template<typename T>
    class brd: public instruction
    {
    public:
        static constexpr const std::string name = "ret";

        //----------------------------------------------------------------------
        ret(const std::vector<std::string>& args) = default;

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            context._position = _frame_stack.top();
            context._frame_stack.pop();
        }
    };
}