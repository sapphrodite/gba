#include "common/integral_types.h"
#include "common/register_types.h"

#include "mmio/display.h"
#include "mmio/palette.h"
#include "mmio/buttons.h"
#include "mmio/object.h"

int main() {
    display::set_mode(5);

    object<0>::set_position(16, 69);
    object<5>::set_position(16, 69);
    object<0>::set_size(object_size::_32x64);
    sprite_palette::set(10, color(31, 16, 0));

    display::active_layers::set(display::layer::BG0, display::layer::BG1, display::layer::OBJ);
    while(!button_states::get(button::RIGHT)) {}

    return 0;
}
