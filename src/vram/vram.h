#ifndef VRAM_H
#define VRAM_H

#include <common/integral_types.h>

struct vram {
    template <typename access_type>
    static access_type* obj_tile_addr(size_t index) { return (access_type*) 0x06010000 + (32 * index); }
};

#endif //VRAM_H
