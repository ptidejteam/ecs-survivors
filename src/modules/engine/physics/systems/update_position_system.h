//
// Created by laurent on 29/05/25.
//

#ifndef UPDATE_POSITION_SYSTEM_H
#define UPDATE_POSITION_SYSTEM_H

#include "modules/engine/physics/physics_module.h"
#include "modules/engine/physics/components.h"

namespace physics {
    inline void update_position_system(const flecs::iter &it, size_t i, core::Position2D &pos, const Velocity2D &vel) {
        pos.value = Vector2Add(
            pos.value, vel.value * std::min(PHYSICS_TICK_LENGTH, it.delta_system_time()));
    }
}

#endif //UPDATE_POSITION_SYSTEM_H
