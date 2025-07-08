//
// Created by laurent on 20/06/25.
//

#ifndef PROJECTILE_BOUNCE_COLLIDED_SYSTEM_H
#define PROJECTILE_BOUNCE_COLLIDED_SYSTEM_H

#include <flecs.h>

#include <raylib.h>
#include <raymath.h>

#include "modules/engine/physics/components.h"
#include "modules/engine/rendering/components.h"
#include "modules/gameplay/components.h"


namespace gameplay::systems {
    inline void projectile_bounce_collided_system(flecs::iter &it, size_t i, physics::CollisionRecordList &list, Bounce &bounce,
                         physics::Velocity2D &vel, rendering::Rotation& rotation) {
        flecs::entity other = it.pair(4).second();
        if (other.get<physics::Collider>().collision_type == physics::environment) {
            physics::CollisionInfo info = list.collisions_info[{it.entity(i).id(), other.id()}];
            vel.value = Vector2Reflect(vel.value, info.normal);
            rotation.angle = Vector2Angle(Vector2{0,1}, Vector2Negate(vel.value)) * RAD2DEG;

            bounce.bounce_count--;
            if (bounce.bounce_count <= 0) {
                it.entity(i).remove<Bounce>();
            }

            it.entity(i).remove<physics::CollidedWith>(other);
        }
    }
}
#endif //PROJECTILE_BOUNCE_COLLIDED_SYSTEM_H
