#ifndef PALETTE_H
#define PALETTE_H

#include <common/integral_types.h>

class color {
public:
    color(u8 r, u8 g, u8 b) { _data = (b << 10) | (g << 5) | r; }
    u8 r() const { return (_data & 0b11111); }
    u8 g() const { return (_data & (0b11111 << 5)) >> 5; }
    u8 b() const { return (_data & (0b11111 << 10)) >> 10; }
private:
    u16 _data = 0;
};

template <uintptr_t address>
class palette {
public:
    static constexpr color& at(size_t index) { return *((color*)address + index); }
};

using background_palette = palette<0x05000000>;
using sprite_palette = palette<0x05000200>;

#endif //PALETTE_H
