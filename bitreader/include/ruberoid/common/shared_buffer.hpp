#pragma once
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <memory>

namespace rb::common
{
    //--------------------------------------------------------------------------
    class shared_buffer
    {
    public:
        //----------------------------------------------------------------------
        shared_buffer(shared_buffer&& other) noexcept: _state(std::move(other._state))
        {

        }

        //----------------------------------------------------------------------
        shared_buffer(const shared_buffer& other) = default;
        ~shared_buffer() = default;

        //----------------------------------------------------------------------
        using iterator = uint8_t*;
        using const_iterator = const uint8_t*;

        size_t size() const { return _state->size; }
        size_t capacity() const { return _state->capacity; }
        const uint8_t* get() const { return _state->data; }
        uint8_t* get() { return _state->data; }

        iterator begin() { return get(); }
        iterator end() { return get() + size(); }
        const_iterator begin() const { return get(); }
        const_iterator end() const { return get() + size(); }
        const_iterator cbegin() const { return get(); }
        const_iterator cend() const { return get() + size(); }

        //----------------------------------------------------------------------
        static shared_buffer wrap_mem(uint8_t* data, size_t size)
        {
            _internal state = {
                .data = data,
                .size = size,
                .capacity = size
            };

            return shared_buffer(state);
        }

        //----------------------------------------------------------------------
        static shared_buffer copy_mem(const uint8_t* data, size_t size)
        {
            _internal state = {
                .data = new uint8_t[size],
                .size = size,
                .capacity = size
            };

            return shared_buffer(state);
        }

        //----------------------------------------------------------------------
        static shared_buffer clone(const shared_buffer& buffer)
        {
            _internal state = {
                .data = new uint8_t[buffer.capacity()],
                .size = buffer.size(),
                .capacity = buffer.capacity()
            };
            std::copy(buffer.get(), buffer.get()+buffer.size(), state.data);
            return shared_buffer(state);
        }

        //----------------------------------------------------------------------
        void resize(size_t new_size)
        {
            auto new_data = new uint8_t[new_size];
            auto copy_size = std::min(new_size, size());
            std::copy(begin(), begin()+copy_size, new_data);
            _internal state = {
                .data = new_data,
                .size = new_size,
                .capacity = new_size
            };

            _state = make_internal(state);
        }

        //----------------------------------------------------------------------
        bool operator bool() const
        {
            return _state.get();
        }

    private:
        //----------------------------------------------------------------------
        struct _internal
        {
            uint8_t* data = nullptr;
            size_t capacity = 0;
            size_t size = 0;
        };

        //----------------------------------------------------------------------
        static std::shared_ptr<_internal> make_internal(_internal state)
        {
            std::shared_ptr<_internal> ret(
                new _internal(),
                [](_internal* ptr) {
                    delete[] ptr->data;
                    ptr->size = 0;
                    ptr->capacity = 0;
                }
            );

            return ret;
        }

        //----------------------------------------------------------------------
        explicit shared_buffer(_internal state): _state(make_internal(state))
        {

        }

        std::shared_ptr<_internal> _state;
    };
}