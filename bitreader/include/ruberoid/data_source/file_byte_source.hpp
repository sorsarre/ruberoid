#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include "ruberoid/common/file_reader.hpp"
#include "ruberoid/common/shared_buffer.hpp"

namespace rb::common
{
    class file_byte_source
    {
    public:
        //----------------------------------------------------------------------
        explicit file_byte_source(std::shared_ptr<file_reader> reader)
            : _reader(std::move(reader))
            , _buffer(shared_buffer::allocate(32*1024)) // TODO: Magic number, replace it
            , _position(0)
            , _last(0)
        {

        }

        //----------------------------------------------------------------------
        uint8_t get()
        {
            if (
                    _position < _last ||
                    _position >= _last+_buffer.size() ||
                    _buffer.size() == 0)
            {
                load_buffer();
            }

            if (_buffer.size() == 0 && available() == 0) {
                throw std::runtime_error("Cannot read beyond the end of the file");
            }
            return _buffer.get()[_position - _last];
        }

        //----------------------------------------------------------------------
        void next()
        {
            if (available() > 0) {
                ++_position;
            }
        }

        //----------------------------------------------------------------------
        bool depleted()
        {
            return _reader->depleted();
        }

        //----------------------------------------------------------------------
        size_t available()
        {
            return _reader->size() - _position;
        }

        //----------------------------------------------------------------------
        void seek(size_t position)
        {
            if (position > _reader->size()) {
                throw std::range_error("Cannot seek beyond file size");
            }

            _position = position;
        }

        //----------------------------------------------------------------------
        void skip(size_t bytes)
        {
            if (bytes > available()) {
                throw std::range_error("Cannot skip beyond file size");
            }

            _position += bytes;
        }

    private:
        //----------------------------------------------------------------------
        void load_buffer()
        {
            auto read = _reader->read(_buffer.get(), _position, _buffer.capacity());
            _buffer.resize(read);
        }

        std::shared_ptr<file_reader> _reader;
        shared_buffer _buffer;
        uint64_t _position;
        uint64_t _last;
    };
}