//
// Created by laurent on 29/05/25.
//

#ifndef RESET_DESIRED_VELOCITY_SYSTEM_H
#define RESET_DESIRED_VELOCITY_SYSTEM_H

#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void reset_desired_velocity_system(const Velocity2D &vel, DesiredVelocity2D &desiredVel) {
        desiredVel.value = vel.value;
    }
}
#endif //RESET_DESIRED_VELOCITY_SYSTEM_H
