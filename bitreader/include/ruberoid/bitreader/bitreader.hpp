#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <stdexcept>

template<typename T>
concept bool FloatingPoint = std::is_floating_point<T>::value;

template<typename T>
concept bool Integral = std::is_integral<T>::value;

template<typename T>
concept bool UnsignedIntegral = Integral<T> && std::is_unsigned<T>::value;

template<typename T>
concept bool SignedIntegral = Integral<T> && !UnsignedIntegral<T>;

template<typename T>
concept bool BitReadable = Integral<T> || FloatingPoint<T>;

namespace rb::common {

    //--------------------------------------------------------------------------
    template<typename T>
    struct bit_read_helper
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 0;
        static constexpr const bool is_signed = std::is_signed<T>::value;
    };

    //--------------------------------------------------------------------------
    template<FloatingPoint T>
    struct bit_read_helper<T>
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 8 * sizeof(T);
        static constexpr const bool is_signed = std::is_signed<T>::value;
    };

    //--------------------------------------------------------------------------
    class bitreader {
    public:
        bitreader() = default;

        //----------------------------------------------------------------------
        void set_data(const uint8_t* data, size_t length) {
            _data = data;
            _data_end = data + length;
            _state.ptr = 0;
            _next(_state);
        }

        //----------------------------------------------------------------------
        template<typename T, size_t Size> T read()
        {
            0 = 0;
        }

        //----------------------------------------------------------------------
        template<UnsignedIntegral T, size_t Size> T read()
        {
            _validate_read_static<T, Size>();

            T ret = 0;
            _read(_state, Size, ret);
            return ret;
        }

        //----------------------------------------------------------------------
        template<SignedIntegral T, size_t Size> T read()
        {
            _validate_read_static<T, Size>();

            T ret = 0;
            _read(_state, Size, ret);
            T m = 1U << (Size - 1);
            return (ret ^ m) - m;
        }

        //----------------------------------------------------------------------
        template<FloatingPoint T, size_t Size> T read()
        {
            _validate_read_static<T, Size>();
            assert(false && "Reading Floating Point Data is not implementer in bitstream");
        }

        //----------------------------------------------------------------------
        void seek(size_t bitpos)
        {
            size_t diff = _data_end - _data;
            if (diff*8 < bitpos) {
                throw std::runtime_error("Can't seek beyond bitstream size");
            }

            _state.ptr = _data;
            _skip(_state, bitpos);
        }

        //----------------------------------------------------------------------
        void align(size_t bits)
        {
            _align(_state, bits);
        }

        //----------------------------------------------------------------------
        void skip(size_t bits)
        {
            _skip(_state, bits);
        }

    private:
        //----------------------------------------------------------------------
        struct internal_state {
            uint64_t buffer = 0;
            size_t shift = 0;
            const uint8_t* ptr = nullptr;
        };

        //----------------------------------------------------------------------
        void _next(internal_state& state) const
        {
            size_t available = std::min<size_t>(
                    sizeof(state.buffer),
                    _data_end - state.ptr);

            size_t to_read = available;
            while (to_read) {
                state.buffer <<= 8;
                state.buffer |= *state.ptr;
                ++state.ptr;
                --to_read;
            }
            state.shift = 8 * available;
        }

        //----------------------------------------------------------------------
        void _skip(internal_state& state, size_t bits) const
        {
            if (_available(state) < bits) {
                throw std::runtime_error("Cannot skip beyond end of bitstream");
            }

            static constexpr const size_t buffer_bits = sizeof(state.buffer) * 8;
            size_t leaps = bits / buffer_bits;
            state.ptr += leaps * buffer_bits;
            _next(state);
            size_t rest = bits % buffer_bits;
            if (rest < state.shift) {
                state.shift -= rest;
            } else if (rest == state.shift) {
                _next(state);
            } else {
                rest -= state.shift;
                _next(state);
                state.shift -= rest;
            }
        }

        //----------------------------------------------------------------------
        size_t _position(internal_state& state) const
        {
            static constexpr const size_t buffer_bits = sizeof(state.buffer) * 8;
            return (state.ptr - _data)*8 + (buffer_bits - state.shift);
        }

        //----------------------------------------------------------------------
        size_t _available(internal_state& state) const
        {
            return (_data_end - state.ptr) * 8 + state.shift;
        }

        //----------------------------------------------------------------------
        void _align(internal_state& state, size_t bits) const
        {
            size_t advance = _position(state) % bits;
            if (advance) {
                _skip(state, advance);
            }
        }

        //----------------------------------------------------------------------
        template<typename T>
        static constexpr T _mask(size_t bits)
        {
            return (T(1) << bits) - 1;
        }

        //----------------------------------------------------------------------
        template<typename T>
        void _elementary_read(internal_state& state, size_t bits, T& ret) const
        {
            state.shift -= bits;
            ret |= (state.buffer >> state.shift) & _mask<T>(bits);
        }

        //----------------------------------------------------------------------
        template<typename T>
        void _read(internal_state& state, size_t bits, T& ret) const
        {
            if (_available(state) < bits) {
                throw std::runtime_error("Cannot read beyond the bitstream data");
            }

            if (bits < state.shift) {
                _elementary_read(state, bits, ret);
            } else if (bits == state.shift) {
                _elementary_read(state, bits, ret);
                _next(state);
            } else {
                bits -= state.shift;
                _elementary_read(state, state.shift, ret);
                _next(state);
                _elementary_read(state, bits, ret);
            }
        }

        //----------------------------------------------------------------------
        template<typename T>
        void _peek(internal_state& state, size_t bits, T& ret) const
        {
            internal_state temporary = state;
            _read(temporary, bits, ret);
        }

        //----------------------------------------------------------------------
        template <typename T, size_t Size>
        constexpr void _validate_read_static() const
        {
            static_assert(Size <= bit_read_helper<T>::max_bits);
            static_assert(Size >= bit_read_helper<T>::min_bits);
        }

        //----------------------------------------------------------------------
        template <typename T>
        void _validate_read_dynamic(size_t size) const
        {
            assert(size <= bit_read_helper<T>::max_bits);
            assert(size >= bit_read_helper<T>::min_bits);
        }

        internal_state _state;
        const uint8_t* _data;
        const uint8_t* _data_end;
    };

}