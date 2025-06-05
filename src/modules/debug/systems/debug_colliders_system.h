//
// Created by laurent on 30/05/25.
//

#ifndef DEBUG_COLLIDERS_SYSTEM_H
#define DEBUG_COLLIDERS_SYSTEM_H

#include <raylib.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace debug::systems {
    inline void debug_circle_colliders_system(const physics::Collider &collider, const core::Position2D &position) {
        DrawCircleLines(position.value.x, position.value.y, collider.radius, GREEN);
    }

    inline void debug_static_colliders_system(const physics::BoxCollider &collider) {
        DrawRectangleLines(collider.rec.x, collider.rec.y, collider.rec.width, collider.rec.height, GREEN);
    }
}
#endif //DEBUG_COLLIDERS_SYSTEM_H
