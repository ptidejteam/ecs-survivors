//
// Created by laurent on 29/05/25.
//

#ifndef SET_HORIZONTAL_INPUT_SYSTEM_H
#define SET_HORIZONTAL_INPUT_SYSTEM_H
#include <flecs.h>
#include <raylib.h>

#include "input/components.h"

namespace input::systems {
    inline void set_horizontal_input_system(const KeyBinding &binding, InputHorizontal &horizontal) {
        if (IsKeyDown(binding.key)) {
            horizontal.value += binding.value;
        }
    }
}
#endif //SET_HORIZONTAL_INPUT_SYSTEM_H
