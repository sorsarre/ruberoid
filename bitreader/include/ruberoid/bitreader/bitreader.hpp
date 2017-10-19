#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <stdexcept>

template<typename T>
using if_unsigned_integral = std::enable_if_t<std::is_unsigned_v<T> && std::is_integral_v<T>, T>;

template<typename T>
using if_signed_integral = std::enable_if_t<std::is_signed_v<T> && std::is_integral_v<T>, T>;

template<typename T>
using if_integral = std::enable_if_t<std::is_integral_v<T>, T>;

template<typename T>
using if_floating_point = std::enable_if_t<std::is_floating_point_v<T>, T>;

template<typename T>
using if_bit_readable = std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, T>;


namespace rb::common {

    //--------------------------------------------------------------------------
    template<typename T>
    struct bit_read_helper
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 0;
        static constexpr const bool is_signed = std::is_signed_v<T>;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    struct floating_point_bit_read_helper
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 8 * sizeof(T);
        static constexpr const bool is_signed = std::is_signed_v<T>;
    };

    //--------------------------------------------------------------------------
    template<> struct bit_read_helper<float>: floating_point_bit_read_helper<float> {};
    template<> struct bit_read_helper<double>: floating_point_bit_read_helper<double> {};
    template<> struct bit_read_helper<long double>: floating_point_bit_read_helper<long double> {};

    //--------------------------------------------------------------------------
    class bitreader {
    public:
        bitreader() = default;

        //----------------------------------------------------------------------
        void set_data(const uint8_t* data, size_t length) {
            _data = data;
            _data_end = data + length;
            _state.ptr = data;
            _next(_state);
        }

        //----------------------------------------------------------------------
        template<typename T>
        if_integral<T> read(size_t bits)
        {
            _validate_read_dynamic<T>(bits);
            T ret = T(0);
            _read(_state, bits, ret);
            return _sign_extend(ret, bits);
        }

        //----------------------------------------------------------------------
        template<typename T>
        if_integral<T> peek(size_t bits)
        {
            _validate_read_dynamic<T>(bits);
            T ret = T(0);
            _peek(_state, bits, ret);
            return _sign_extend(ret, bits);
        }


        //----------------------------------------------------------------------
        size_t position() const
        {
            return _position(_state);
        }

        //----------------------------------------------------------------------
        size_t available() const
        {
            return _available(_state);
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
        template<typename T>
        if_signed_integral<T> _sign_extend(T raw, size_t bits)
        {
            T m = 1U << (bits - 1);
            return (raw ^ m) - m;
        }

        //----------------------------------------------------------------------
        template<typename T>
        if_unsigned_integral<T> _sign_extend(T raw, size_t bits)
        {
            return raw;
        }

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

            if (bits < state.shift) {
                state.shift -= bits;
            } else if (bits == state.shift) {
                _next(state);
            } else {
                size_t to_skip = bits - state.shift;
                state.shift = 0;
                state.ptr += to_skip / 8;
                _next(state);
                state.shift -= to_skip % 8;
            }
        }

        //----------------------------------------------------------------------
        size_t _position(const internal_state& state) const
        {
            return (_state.ptr - _data) * 8 - state.shift;
        }

        //----------------------------------------------------------------------
        size_t _available(const internal_state& state) const
        {
            return (_data_end - state.ptr) * 8 + state.shift;
        }

        //----------------------------------------------------------------------
        void _align(internal_state& state, size_t bits) const
        {
            // TODO: Rewrite
            size_t advance = (bits - (_position(state) % bits)) % bits;
            if (advance) {
                _skip(state, advance);
            }
        }

        //----------------------------------------------------------------------
        template<typename T>
        static constexpr T _mask(size_t bits)
        {
            return (bits == sizeof(T)*8) ? (~0) : ((T(1) << bits) - 1);
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
                ret <<= bits;
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