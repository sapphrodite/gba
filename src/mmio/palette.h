#ifndef PALETTE_H
#define PALETTE_H

#include <common/mmio_array.h>

class color {
public:
    constexpr color(u8 r, u8 g, u8 b) { _data = to_rgb16(r, g, b); }
    constexpr color(u16 value) { _data = value; }

    constexpr void set(u8 r, u8 g, u8 b) volatile { _data = to_rgb16(r, g, b); }
    constexpr void set(u16 data) volatile { _data = data; }

    u8 r() volatile { return (_data & 0b11111); }
    u8 g() volatile { return (_data & (0b11111 << 5)) >> 5; }
    u8 b() volatile { return (_data & (0b11111 << 10)) >> 10; }
private:
    constexpr static u16 to_rgb16(u8 r, u8 g, u8 b) { return (b << 10) | (g << 5) | r; }
    u16 _data;
};

using background_palette = mmio_array<mmio<0x05000000, u8>, color>;
using sprite_palette = mmio_array<mmio<0x05000200, u8>, color>;

#endif //PALETTE_H
