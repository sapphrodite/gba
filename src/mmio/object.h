#ifndef OBJECT_H
#define OBJECT_H

#include <common/integral_types.h>
#include <common/register_types.h>

enum object_size {
    _8x8, _16x16, _32x32, _64x64,
    _16x8, _32x8, _32x16, _64x32,
    _8x16, _8x32, _16x32, _32x64,
};

class object {
public:
    void set_position(u8 x, u8 y) volatile {
        mmio_functions::set_int(&_attrib_0, _position_bitmask, y);
        mmio_functions::set_int(&_attrib_1, _position_bitmask, x);
    }
    void set_size(object_size size) volatile {
        mmio_functions::set_int(&_attrib_0, _obj_size_bitmask, size >> 2);
        mmio_functions::set_int(&_attrib_1, _obj_size_bitmask, size & 0x3);
    }
    void disable() volatile {
        mmio_functions::set_int(&_attrib_0, 0x300, 0b10);
    }
    //void set_mapmode()
private:
    u16 _attrib_0;
    u16 _attrib_1;
    u16 _attrib_2;
    std::byte _storage[2];

    constexpr static size_t _obj_size_bitmask = 0xC000;
    constexpr static size_t _position_bitmask = 0xF;
};

using objects = mmio_array<mmio<0x07000000, u8>, object>;

#endif //OBJECT_H
