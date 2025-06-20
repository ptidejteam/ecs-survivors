//
// Created by laurent on 29/05/25.
//

#ifndef UPDATE_VELOCITY_SYSTEM_H
#define UPDATE_VELOCITY_SYSTEM_H
#include <raymath.h>

#include "modules/engine/physics/physics_module.h"
#include "modules/engine/physics/components.h"

namespace physics::systems {

    inline void update_velocity_system(flecs::iter &it, size_t, Velocity2D &vel, const DesiredVelocity2D &desiredVel,
                          const AccelerationSpeed &acceleration_speed) {
        // eventually I want to use spherical linear interpolation for a smooth transition
        float dt = std::min(PHYSICS_TICK_LENGTH, it.delta_system_time());
        vel.value = Vector2Lerp(vel.value, desiredVel.value,acceleration_speed.value * dt);
        if (Vector2Length(vel.value) < 0.001) vel.value = {0,0};
    }

}
#endif //UPDATE_VELOCITY_SYSTEM_H
