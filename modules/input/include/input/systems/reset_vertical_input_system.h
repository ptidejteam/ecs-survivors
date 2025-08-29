//
// Created by laurent on 29/05/25.
//

#ifndef RESET_VERTICAL_INPUT_SYSTEM_H
#define RESET_VERTICAL_INPUT_SYSTEM_H

#include "input/components.h"

namespace input::systems {
    inline void reset_vertical_input_system(InputVertical& vertical) {
        vertical.value = 0.0f;
    }
}

#endif //RESET_VERTICAL_INPUT_SYSTEM_H
