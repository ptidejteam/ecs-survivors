//
// Created by laurent on 29/05/25.
//

#ifndef UPDATE_POSITION_SYSTEM_H
#define UPDATE_POSITION_SYSTEM_H

#include "modules/engine/physics/physics_module.h"
#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void update_position_system(const flecs::iter &it, size_t i, core::Position2D &pos, const Velocity2D &vel) {
        // TODO: change back to delta_system_time once bug is figured out
        // for some reason, if a system call from a tick source, the system_delta_time does not
        // get affected by time_scale(0)
        if(it.world().get<core::Paused>().paused) return;
        float dt = std::min(PHYSICS_TICK_LENGTH, it.delta_time());

        pos.value = Vector2Add(
            pos.value, vel.value * PHYSICS_TICK_LENGTH);
    }
}

#endif //UPDATE_POSITION_SYSTEM_H
