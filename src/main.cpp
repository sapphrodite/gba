#include "common/integral_types.h"
#include "common/register_types.h"

#include "mmio/display.h"
#include "mmio/palette.h"
#include "mmio/buttons.h"
#include "mmio/object.h"

int main() {
    display::bg_mode::set(5);
    display::active_layers::set(display::layer::BG0, display::layer::BG1, display::layer::OBJ);
    display::obj_mapping::set(display::obj_mapping_mode::_1d);

    auto& obj1 = objects::get(0);
    obj1.set_position(53, 69);
    obj1.set_size(object_size::_16x32);

    auto& j = sprite_palette::get(10);
    j.set(31, 16, 0);

    while(!button_states::get(button::RIGHT)) {}

    return 0;
}
