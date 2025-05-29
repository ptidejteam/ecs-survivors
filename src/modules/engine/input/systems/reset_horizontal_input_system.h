//
// Created by laurent on 29/05/25.
//

#ifndef RESET_HORIZONTAL_INPUT_SYSTEM_H
#define RESET_HORIZONTAL_INPUT_SYSTEM_H
#include "modules/engine/input/components.h"

namespace input {
    inline void reset_horizontal_input_system(InputHorizontal& horizontal) {
        horizontal.value = 0.0f;
    }
}

#endif //RESET_HORIZONTAL_INPUT_SYSTEM_H
