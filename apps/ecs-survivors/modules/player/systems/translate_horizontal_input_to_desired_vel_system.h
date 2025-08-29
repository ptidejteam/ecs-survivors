//
// Created by laurent on 30/05/25.
//

#ifndef TRANSLATE_HORIZONTAL_INPUT_TO_DESIRED_VEL_SYSTEM_H
#define TRANSLATE_HORIZONTAL_INPUT_TO_DESIRED_VEL_SYSTEM_H
#include "input/components.h"
#include "physics/components.h"

namespace player::systems {
    inline void translate_horizontal_input_to_desired_vel_system(const input::InputHorizontal &horizontal,
                                                                 physics::DesiredVelocity2D &desired_vel) {
        desired_vel.value.x = horizontal.value;
    }
}
#endif //TRANSLATE_HORIZONTAL_INPUT_TO_DESIRED_VEL_SYSTEM_H
