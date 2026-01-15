//
// Created by laurent on 29/05/25.
//

#ifndef SET_VERTICAL_INPUT_SYSTEM_H
#define SET_VERTICAL_INPUT_SYSTEM_H

#include <raylib.h>

#include "input/components.h"

namespace input::systems {
    inline void set_vertical_input_system(const KeyBinding &binding, InputVertical &vertical) {
        if (IsKeyDown(binding.key)) {
            vertical.value += binding.value;
        }
    }
}
#endif //SET_VERTICAL_INPUT_SYSTEM_H
