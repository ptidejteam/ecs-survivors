//
// Created by laurent on 20/06/25.
//

#ifndef DEBUG_COLLIDABLE_ENTITIES_SYSTEM_H
#define DEBUG_COLLIDABLE_ENTITIES_SYSTEM_H


#include <flecs.h>

#include "core/components.h"
#include "physics/components.h"

namespace debugging::systems {
    inline void debug_collidable_entities_system(flecs::entity e, const core::Position2D &pos, const physics::Collider &collider) {
        DrawText(
            std::to_string(e.id()).c_str(),
            pos.value.x + collider.bounds.x + 12,
            pos.value.y + collider.bounds.y + 12, 16, GREEN);
    }
}
#endif //DEBUG_COLLIDABLE_ENTITIES_SYSTEM_H
