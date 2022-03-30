#ifndef PALETTE_H
#define PALETTE_H

#include <common/integral_types.h>

class color {
public:
    constexpr color(u8 r, u8 g, u8 b) { _data = (b << 10) | (g << 5) | r; }
    constexpr color(u16 value) { _data = value; }
    constexpr u16 rgb() { return _data; }
    u8 r() const { return (_data & 0b11111); }
    u8 g() const { return (_data & (0b11111 << 5)) >> 5; }
    u8 b() const { return (_data & (0b11111 << 10)) >> 10; }
private:
    u16 _data = 0;
};

template <uintptr_t address>
class palette {
public:
    static constexpr void set(size_t index, color c) { _impl::memory[index] = c.rgb(); }
    static constexpr color get(size_t index) { return color(_impl::memory[index]); }
private:
    using _impl = mmio<address, u16>;
};

using background_palette = palette<0x05000000>;
using sprite_palette = palette<0x05000200>;

#endif //PALETTE_H
