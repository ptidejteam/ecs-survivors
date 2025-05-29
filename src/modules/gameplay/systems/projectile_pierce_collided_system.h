//
// Created by laurent on 29/05/25.
//

#ifndef PROJECTILE_PIERCE_COLLIDED_SYSTEM_H
#define PROJECTILE_PIERCE_COLLIDED_SYSTEM_H

#include <flecs.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"
#include "modules/gameplay/components.h"

namespace gameplay {
    inline void projectile_pierce_collided_system(flecs::iter &it, size_t i, Pierce &pierce) {
        flecs::entity other = it.pair(1).second();
        if (pierce.hits.contains(other.id())) {
            it.entity(i).remove<physics::CollidedWith>(other);
            return;
        }
        pierce.hits.insert(other.id());
        pierce.pierce_count -= 1;
        if (pierce.pierce_count < 0) {
            it.entity(i).add<core::DestroyAfterFrame>();
        }
    }
}
#endif //PROJECTILE_PIERCE_COLLIDED_SYSTEM_H
