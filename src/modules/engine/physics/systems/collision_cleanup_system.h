//
// Created by laurent on 29/05/25.
//

#ifndef COLLISION_CLEANUP_SYSTEM_H
#define COLLISION_CLEANUP_SYSTEM_H

#include <flecs.h>

#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void collision_cleanup_system(flecs::iter &it, size_t i) {
        flecs::entity self = it.entity(i);
        flecs::entity other = it.pair(0).second();
        self.remove<CollidedWith>(other);
    }
}

#endif //COLLISION_CLEANUP_SYSTEM_H
