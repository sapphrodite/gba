#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include "integral_types.h"
#include <bit>


template <uintptr_t address, typename register_datatype>
class mmio {
public:
    static constinit inline volatile register_datatype* const memory = reinterpret_cast<volatile register_datatype*>(address);
    static_assert((address % sizeof(register_datatype) == 0), "Input is improperly aligned");
};

template <uintptr_t address, typename register_datatype, register_datatype bitmask>
class register_base : public mmio<address, register_datatype> {
public:
    static constexpr u8 shift_amount = std::countr_zero(bitmask);
};

template <uintptr_t address, typename register_datatype, register_datatype bitmask, bool readable, bool writable, typename access_type = register_datatype>
class integer_register {
public:
    static void clear() {
        static_assert(writable, "Cannot write to this register");
        *base::memory = *base::memory & bitmask;
    };

    static void write(access_type value) {
        static_assert(writable, "Cannot write to this register");
        clear();
        *base::memory = *base::memory | (register_datatype)value << base::shift_amount;
    };

    static access_type read() {
        static_assert(readable, "Cannot read from this register");
        return (access_type)((*base::memory & bitmask) >> base::shift_amount);
    };
private:
    using base = register_base<address, register_datatype, bitmask>;
};

template <uintptr_t address, typename register_datatype, register_datatype controlled_bits, bool readable, bool writable, typename access_type = register_datatype>
class bitfield_register {
public:
    static bool get(access_type index) {
        static_assert(readable, "Cannot read from this register");
        return (*base::memory & _bitmask(index)) == _bitmask(index);
    };

    template <typename... Args>
    static void set(Args... args) {
        static_assert(readable, "Cannot write to this register");
        *base::memory = *base::memory | (... | _bitmask(args));
    };

    template <typename... Args>
    static void clear(access_type index, Args... args) {
        static_assert(readable, "Cannot write to this register");
        *base::memory = *base::memory & ~(... | _bitmask(args));
    };
private:
    constexpr static register_datatype _bitmask(access_type index) {
        return 1 << (register_datatype) index << base::shift_amount;
    }
    using base = register_base<address, register_datatype, controlled_bits>;
};

#endif //REGISTER_TYPES_H
