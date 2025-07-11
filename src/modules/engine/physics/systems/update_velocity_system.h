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
        // TODO: change back to delta_system_time once bug is figured out
        // for some reason, if a system call from a tick source, the system_delta_time does not
        // get affected by time_scale(0)
        if(it.world().get<core::Paused>().paused) return;
        float dt = std::min(PHYSICS_TICK_LENGTH, it.delta_time());
        vel.value = Vector2Lerp(vel.value, desiredVel.value,acceleration_speed.value * PHYSICS_TICK_LENGTH);
        if (Vector2Length(vel.value) < 0.001) vel.value = {0,0};
    }

}
#endif //UPDATE_VELOCITY_SYSTEM_H
