#ifndef OBJECT_H
#define OBJECT_H

#include <common/integral_types.h>
#include <common/register_types.h>


enum object_size {
    _8x8, _16x16, _32x32, _64x64,
    _16x8, _32x8, _32x16, _64x32,
    _8x16, _8x32, _16x32, _32x64,
};

// Also shared with attribute 0's OBJ_SHAPE field
constexpr static size_t obj_size_bitmask() { return 0xC000; }

template <size_t index>
class object {
public:
    static void set_position(u8 x, u8 y) {
        *_a::memory = *_a::memory & 0x0F;
        *_a::memory = *_a::memory | y;

        *_b::memory = *_b::memory & 0x0F;
        *_b::memory = *_b::memory | x;
    }
    static void set_size(object_size size) {
        constexpr size_t shift_amount = std::countr_zero(obj_size_bitmask());

        *_a::memory = *_a::memory & ~obj_size_bitmask();
        *_a::memory = *_a::memory | ((size_t)size >> 2) << shift_amount;


        *_b::memory = *_b::memory & ~obj_size_bitmask();
        *_b::memory = *_b::memory | ((size_t)size & 0x3) << shift_amount;
    }
private:
    using _a = mmio<0x07000000 + (8 * index), u16>;
    using _b = mmio<0x07000002 + (8 * index), u16>;
    using _c = mmio<0x07000004 + (8 * index), u16>;
};

#endif //OBJECT_H
