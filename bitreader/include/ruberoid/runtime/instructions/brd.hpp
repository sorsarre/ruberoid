#pragma once
#include <boost/lexical_cast.hpp>
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
        static const std::string name() {
            return std::string("brd") +
                   (std::is_signed_v<T> ? "s" : "u") +
                   std::string(bit_string<T>());
        }

        //----------------------------------------------------------------------
        brd(const std::vector<std::string>& args)
        {
            _target = args.at(0);
            _bits = boost::lexical_cast<size_t>(args.at(1));
        }

        //----------------------------------------------------------------------
        void exec(execution_context& context, common::bitreader& reader) override
        {
            auto value = context._context_stack.resolve(_target);
            auto data = reader.read<T>(_bits);
            value->set(data);
        }

        //----------------------------------------------------------------------
        std::string to_string() override
        {
            std::ostringstream oss;
            oss << name() << " " << _target << " " << _bits;
            return oss.str();
        }

    private:
        size_t _bits;
        std::string _target;

    };
}