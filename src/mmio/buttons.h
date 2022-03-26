#ifndef BUTTONS_H
#define BUTTONS_H

#include <common/register_types.h>

enum class button {
    A, B,
    SELECT, START,
    RIGHT, LEFT, UP, DOWN,
    R, L
};

struct button_states {
    static bool get(button b) { return ~_impl::get(b); }
private:
    using _impl = bitfield_register<0x04000130, u16, 0x3FF, true, false, button>;
};

#endif //BUTTONS_H
