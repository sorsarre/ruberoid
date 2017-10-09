#pragma once
#include <cstdio>
#include <cstdint>
#include <cstddef>

#include <stdexcept>
#include <string>
#include <memory>

#include "ruberoid/common/file_reader.hpp"

namespace rb::common
{
    class direct_file_reader: public file_reader
    {
    public:
        //----------------------------------------------------------------------
        size_t read(uint8_t* dest, uint64_t position, size_t bytes) override
        {
            if (fseeko64(_file, position, SEEK_SET) < 0) {
                throw std::runtime_error("Could not seek to designated position");
            }

            return fread(dest, 1, bytes, _file);
        }

        //----------------------------------------------------------------------
        uint64_t size() override
        {
            fseeko64(_file, 0, SEEK_END);
            auto result = ftello64(_file);
            if (result < 0) {
                throw std::runtime_error("Could not seek to the end of the file");
            }

            return static_cast<uint64_t>(result);
        }

        //----------------------------------------------------------------------
        bool depleted() override
        {
            return true;
        }

        //----------------------------------------------------------------------
        ~direct_file_reader() override
        {
            fclose(_file);
        }

        //----------------------------------------------------------------------
        static std::shared_ptr<file_reader> open(const std::string& path)
        {
            return std::shared_ptr<file_reader>(new direct_file_reader(path));
        }

    private:
        //----------------------------------------------------------------------
        explicit direct_file_reader(const std::string& path)
        {
            _file = fopen64(path.c_str(), "rb");
            if (!_file) {
                throw std::runtime_error("Could not open file for reading");
            }
        }

        FILE* _file;
    };
}