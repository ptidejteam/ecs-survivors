//
// Created by laurent on 20/06/25.
//

#ifndef PROJECTILE_NO_BOUNCE_COLLIDED_SYSTEM_H
#define PROJECTILE_NO_BOUNCE_COLLIDED_SYSTEM_H

#include <flecs.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"


namespace gameplay::systems {
    inline void projectile_no_bounce_collided_system(flecs::iter &it, size_t i) {
        flecs::entity other = it.pair(0).second();
        if (other.get<physics::Collider>().collision_type == physics::environment) {
            it.entity(i).remove<physics::CollidedWith>(other);
            it.entity(i).add<core::DestroyAfterFrame>();
        }
    }
}
#endif //PROJECTILE_NO_BOUNCE_COLLIDED_SYSTEM_H
