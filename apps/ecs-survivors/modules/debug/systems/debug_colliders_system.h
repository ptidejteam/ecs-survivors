//
// Created by laurent on 30/05/25.
//

#ifndef DEBUG_COLLIDERS_SYSTEM_H
#define DEBUG_COLLIDERS_SYSTEM_H

#include <raylib.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace debug::systems {

    inline void debug_collider_bounds_system(const core::Position2D &pos, const physics::Collider &collider) {
        DrawRectangleLines(pos.value.x + collider.bounds.x, pos.value.y + collider.bounds.y,
                           collider.bounds.width, collider.bounds.height, MAGENTA);

    }

    inline void debug_circle_colliders_system(const physics::CircleCollider &collider,
                                              const core::Position2D &position) {
        DrawCircleLines(position.value.x, position.value.y, collider.radius, GREEN);
    }

    inline void debug_box_colliders_system(const core::Position2D &pos,
                                              const physics::Collider &collider) {

        DrawRectangleLines(pos.value.x + collider.bounds.x, pos.value.y + collider.bounds.y, collider.bounds.width,
                           collider.bounds.height, GREEN);
    }
}
#endif //DEBUG_COLLIDERS_SYSTEM_H
