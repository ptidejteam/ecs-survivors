//
// Created by laurent on 30/05/25.
//

#ifndef SCALE_DESIRED_VELOCITY_SYSTEM_H
#define SCALE_DESIRED_VELOCITY_SYSTEM_H
#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"
#include <raymath.h>

namespace player::systems {
    inline void scale_desired_velocity_system(physics::DesiredVelocity2D &velocity,
                         const core::Speed &speed) {
        velocity.value = Vector2Normalize(velocity.value) * speed.value;
    }
}
#endif //SCALE_DESIRED_VELOCITY_SYSTEM_H
