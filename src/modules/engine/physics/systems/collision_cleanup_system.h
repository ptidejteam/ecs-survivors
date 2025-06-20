//
// Created by laurent on 29/05/25.
//

#ifndef COLLISION_CLEANUP_SYSTEM_H
#define COLLISION_CLEANUP_SYSTEM_H

#include <flecs.h>

#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void collision_cleanup_system(flecs::iter &it, size_t i, Collider& collider) {
        it.world().remove_all<CollidedWith>(it.entity(i));
    }
}

#endif //COLLISION_CLEANUP_SYSTEM_H
