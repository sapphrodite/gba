#ifndef OBJECT_H
#define OBJECT_H

#include <common/integral_types.h>
#include <common/register_types.h>

template <size_t index>
class object {
public:
    static void set_position(u8 x, u8 y) {
        *_a::memory = *_a::memory & 0x0F;
        *_a::memory = *_a::memory | y;

        *_b::memory = *_b::memory & 0x0F;
        *_b::memory = *_b::memory | x;
    }
private:
    using _a = register_base<0x07000000 + (8 * index), u16, 0xFFFF>;
    using _b = register_base<0x07000002 + (8 * index), u16, 0xFFFF>;
    using _c = register_base<0x07000004 + (8 * index), u16, 0xFFFF>;
};

#endif //OBJECT_H
