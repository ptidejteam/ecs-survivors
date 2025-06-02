//
// Created by laurent on 30/05/25.
//

#ifndef FOLLOW_TARGET_SYSTEM_H
#define FOLLOW_TARGET_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace ai::systems {
    inline void follow_target_system(flecs::iter &it, size_t i,
                              const core::Position2D &position,
                              const core::Speed &speed,
                              physics::DesiredVelocity2D &velocity) {
        const flecs::entity target = it.pair(3).second(); // target component
        if (target.id() == 0) return;
        const Vector2 dir = Vector2Normalize(target.get<core::Position2D>()->value - position.value);
        velocity.value = dir * speed.value;
    }
}
#endif //FOLLOW_TARGET_SYSTEM_H
