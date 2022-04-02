#ifndef DISPLAY_H
#define DISPLAY_H

#include <common/integral_types.h>
#include <common/register_types.h>

class display {
    using DISPCNT = mmio<0x04000000, u16>;
public:
    enum class layer {
        BG0,
        BG1,
        BG2,
        BG3,
        OBJ
    };

    using active_layers = bitfield_register<DISPCNT, 0x1F00, layer>;
    using bg_mode = integer_register<DISPCNT, 0x7>;

    enum class obj_mapping_mode {
        _2d, _1d
    };
    using obj_mapping = integer_register<DISPCNT, 0x40, obj_mapping_mode>;
};

#endif //DISPLAY_H