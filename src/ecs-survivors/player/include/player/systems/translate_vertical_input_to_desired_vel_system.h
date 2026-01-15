//
// Created by laurent on 30/05/25.
//

#ifndef TRANSLATE_VERTICAL_INPUT_TO_DESIRED_VEL_SYSTEM_H
#define TRANSLATE_VERTICAL_INPUT_TO_DESIRED_VEL_SYSTEM_H
#include "input/components.h"
#include "physics/components.h"

namespace player::systems {
    inline void translate_vertical_input_to_desired_vel_system(const input::InputVertical &vertical,
                                                               physics::DesiredVelocity2D &desired_vel) {
        desired_vel.value.y = vertical.value;
    }
}
#endif //TRANSLATE_VERTICAL_INPUT_TO_DESIRED_VEL_SYSTEM_H
