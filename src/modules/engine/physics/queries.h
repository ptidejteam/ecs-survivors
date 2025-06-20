//
// Created by laurent on 30/05/25.
//

#ifndef PHYSICS_QUERIES_H
#define PHYSICS_QUERIES_H

#include <flecs.h>

#include "components.h"
#include "modules/engine/core/components.h"

namespace physics::queries {
    inline flecs::query<core::Position2D, Collider> visible_collision_bodies_query;
    inline flecs::query<core::Position2D, Collider> box_collider_query;
}
#endif //PHYSICS_QUERIES_H
