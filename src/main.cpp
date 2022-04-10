#include "common/integral_types.h"
#include "common/register_types.h"

#include "mmio/display.h"
#include "mmio/palette.h"
#include "mmio/buttons.h"
#include "mmio/object.h"

extern "C" const std::byte _binary_bun_tex_start[];
extern "C" const std::byte _binary_bun_pal_start[];

int main() {
    auto& obj1 = objects::get(0);
    obj1.set_position(53, 69);
    obj1.set_size(object_size::_16x16);
    for (int i = 1; i < 128; i++) {
        objects::get(i).disable();
    }

    u16* bun_pal = (u16*) _binary_bun_pal_start;
    for (int i = 0; i < 7; i++) {
        sprite_palette::get(i).set(bun_pal[i]);
    }

    volatile u16* foo = (u16*)0x06010000;
    u16* bun_tex = (u16*) _binary_bun_tex_start;
    for (int i = 0; i < 128; i++) {
        foo[i] = bun_tex[i];
    }

    display::bg_mode::set(0);
    display::force_blank::set(false);
    display::active_layers::set(display::layer::OBJ);
    display::obj_mapping::set(display::obj_mapping_mode::_1d);

    while(!button_states::get(button::RIGHT)) {}

    return 0;
}
