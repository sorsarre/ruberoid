#pragma once

#include <boost/lexical_cast.hpp>
#include "ruberoid/runtime/instruction.hpp"

namespace rb::runtime::instructions
{
    class bskp: public instruction
    {
    public:
        static const std::string name() { return "bskp"; }

        //----------------------------------------------------------------------
        bskp(const std::vector<std::string>& args)
        {
            _bits = boost::lexical_cast<size_t>(args.at(0));
        }

        //----------------------------------------------------------------------
        virtual void exec(execution_context& context, common::bitreader& reader)
        {
            reader.skip(_bits);
        }

        //----------------------------------------------------------------------
        std::string to_string() override
        {
            std::ostringstream oss;
            oss << name() << " " << _bits;
            return oss.str();
        }

    private:
        size_t _bits;
    };
}