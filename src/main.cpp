#include <cstdint>

#include "common/integral_types.h"
#include "common/register_types.h"
#include "mmio/display.h"
#include "mmio/palette.h"
#include "mmio/buttons.h"
#include "mmio/object.h"

extern "C" const u16 bun_tex[];
extern "C" const u16 bun_pal[];

extern "C" const size_t bun_pal_size;
extern "C" const size_t bun_tex_size;

int main() {
    auto& obj1 = objects::get(0);
    obj1.set_position(53, 69);
    obj1.set_size(object_size::_16x16);

    for (int i = 1; i < 128; i++) {
        objects::get(i).disable();
    }

    int pal_size = reinterpret_cast<std::uintptr_t>(&bun_pal_size) / sizeof(u16);
    for (int i = 0; i < pal_size; i++) {
        sprite_palette::get(i).set(bun_pal[i]);
    }

    int tex_size = reinterpret_cast<std::uintptr_t>(&bun_tex_size) / sizeof(u16);
    volatile u16* foo = (u16*)0x06010000;
    for (int i = 0; i < tex_size; i++) {
        foo[i] = bun_tex[i];
    }

    display::bg_mode::set(0);
    display::force_blank::set(false);
    display::active_layers::set(display::layer::OBJ);
    display::obj_mapping::set(display::obj_mapping_mode::_1d);

    while(!button_states::get(button::RIGHT)) {}

    return 0;
}
