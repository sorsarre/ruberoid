#pragma once
#include <string>
#include <stdexcept>

#include "ruberoid/common/shared_buffer.hpp"

namespace rb::common
{
    class memory_byte_source
    {
    public:
        //----------------------------------------------------------------------
        memory_byte_source(const uint8_t* data, size_t size)
            : _data(shared_buffer::copy_mem(data, size))
        {
            _current = _data.begin();
        }

        //----------------------------------------------------------------------
        uint8_t get()
        {
            if (_current == _data.end()) {
                throw std::runtime_error("Access beyond data buffer boundaries");
            }

            return *_current;
        }

        //----------------------------------------------------------------------
        void next()
        {
            if (_current != _data.end()) {
                ++_current;
            }
        }

        //----------------------------------------------------------------------
        bool depleted()
        {
            return true; // Memory buffer source is always depleted
            // I.e. never gets any additional data
        }

        //----------------------------------------------------------------------
        size_t available()
        {
            return _data.end() - _current;
        }

        //----------------------------------------------------------------------
        void seek(size_t position)
        {
            if (position > _data.size()) {
                throw std::range_error("Position outside of the data buffer");
            }

            _current = _data.begin() + position;
        }

        //----------------------------------------------------------------------
        void skip(size_t bytes)
        {
            if (bytes > available()) {
                throw std::range_error("Cannot skip beyond the boundaries of the data buffer");
            }

            _current += bytes;
        }

    private:
        shared_buffer::iterator _current;
        shared_buffer _data;
    };
}
