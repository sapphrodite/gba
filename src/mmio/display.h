#ifndef DISPLAY_H
#define DISPLAY_H

#include <common/integral_types.h>
#include <common/register_types.h>

class display {
public:
    enum class layer {
        BG0,
        BG1,
        BG2,
        BG3,
        OBJ
    };

    using active_layers = bitfield_register<0x04000000, u16, 0b0001111100000000, true, true, layer>;


    static void set_mode(u8 mode) { display_mode::write(mode); }
private:
    using display_mode = integer_register<0x04000000, u16, 0b111, true, true>;
};

#endif //DISPLAY_H