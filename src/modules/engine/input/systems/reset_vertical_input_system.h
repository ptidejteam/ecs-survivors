//
// Created by laurent on 29/05/25.
//

#ifndef RESET_VERTICAL_INPUT_SYSTEM_H
#define RESET_VERTICAL_INPUT_SYSTEM_H

#include "modules/engine/input/components.h"

namespace input {
    inline void reset_vertical_input_system(InputVertical& vertical) {
        vertical.value = 0.0f;
    }
}

#endif //RESET_VERTICAL_INPUT_SYSTEM_H
