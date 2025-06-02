//
// Created by laurent on 30/05/25.
//

#ifndef STOP_WHEN_ARRIVED_AT_TARGET_SYSTEM_H
#define STOP_WHEN_ARRIVED_AT_TARGET_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "modules/ai/components.h"
#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace ai::systems {
    inline void stop_when_arrived_at_target_system(flecs::iter &it, size_t i,
                              const StoppingDistance &distance,
                              const core::Position2D &pos,
                              physics::DesiredVelocity2D &velocity) {
        const flecs::entity target = it.pair(3).second(); // target component
        if (target.id() == 0) return;
        const Vector2 ab = target.get<core::Position2D>()->value - pos.value;

        // using the squared length is faster computationally
        const float distSquared = Vector2LengthSqr(ab);

        // square the distance
        if (distSquared < distance.value * distance.value) {
            velocity.value = {0, 0};
        }
    }
}
#endif //STOP_WHEN_ARRIVED_AT_TARGET_SYSTEM_H
