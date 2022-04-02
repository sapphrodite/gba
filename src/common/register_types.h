#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include "integral_types.h"
#include <bit>



namespace mmio_functions {
    template <typename T>
    static void clear_int(T* const memory, unsigned owned_bits) {
        *memory = *memory & ~owned_bits;
    };

    template <typename T>
    static void set_int(T* const memory, unsigned owned_bits, unsigned value) {
        *memory = (*memory & ~owned_bits) | (value << std::countr_zero(owned_bits));
    };
}


template <uintptr_t address, typename datatype_in, bool readable_in = true, bool writable_in = true>
class mmio {
public:
    using datatype = datatype_in;

    static constexpr inline bool writable = writable_in;
    static constexpr inline bool readable = readable_in;
    static constinit inline volatile datatype* const memory = reinterpret_cast<volatile datatype*>(address);
    static_assert((address % sizeof(datatype) == 0), "Input is improperly aligned");
};


template <typename mmio, typename mmio::datatype owned_bits, typename access_type = typename mmio::datatype>
class integer_register {
public:
    static void clear() {
        static_assert(mmio::writable, "Cannot write to this register");
        mmio_functions::clear_int(mmio::memory, owned_bits);
    };

    static void set(access_type value) {
        static_assert(mmio::writable, "Cannot write to this register");
        mmio_functions::set_int(mmio::memory, owned_bits, (unsigned) value);
    };

    static access_type get() {
        static_assert(mmio::readable, "Cannot read from this register");
        return (access_type)((*mmio::memory & owned_bits) >> shift_amount);
    };
private:
    static constexpr unsigned shift_amount = std::countr_zero(owned_bits);
};

template <typename mmio, typename mmio::datatype owned_bits, typename access_type = typename mmio::datatype>
class bitfield_register {
public:
    static bool get(access_type index) {
        static_assert(mmio::readable, "Cannot read from this register");
        return (*mmio::memory & _bitmask(index)) == _bitmask(index);
    };

    template <typename... Args>
    static void set(Args... args) {
        static_assert(mmio::readable, "Cannot write to this register");
        *mmio::memory = *mmio::memory | (... | _bitmask(args));
    };

    template <typename... Args>
    static void clear(access_type index, Args... args) {
        static_assert(mmio::readable, "Cannot write to this register");
        *mmio::memory = *mmio::memory & ~(... | _bitmask(args));
    };
private:
    constexpr static typename mmio::datatype _bitmask(access_type index) {
        return 1 << (typename mmio::datatype) index << std::countr_zero(owned_bits);
    }
};


#endif //REGISTER_TYPES_H
