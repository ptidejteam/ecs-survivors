//
// Created by laurent on 29/05/25.
//

#ifndef PROJECTILE_SPLIT_COLLISION_SYSTEM_H
#define PROJECTILE_SPLIT_COLLISION_SYSTEM_H
#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/rendering/components.h"
#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void projectile_split_collision_system(flecs::iter &it, size_t i, Split &split, physics::Velocity2D &vel, core::Position2D &pos,
                                  rendering::Rotation &rot, Attack &attack) {
        flecs::entity other = it.pair(5).second();

        if (split.hits.contains(other.id())) {
            it.entity(i).remove<physics::CollidedWith>(other);
            return;
        }
        split.hits.insert(other.id());

        Vector2 left = Vector2Rotate(vel.value, -90 * DEG2RAD);
        Vector2 right = Vector2Rotate(vel.value, 90 * DEG2RAD);

        auto prefab = it.world().lookup(attack.attack_prefab_name.c_str());
        it.world().entity().is_a(prefab).child_of(it.entity(i).parent())
                .set<core::Position2D>(pos)
                .set<rendering::Rotation>({
                    rot.angle - 90.0f
                })
                .set<physics::Velocity2D>({
                    {left}
                }).remove<Split>().remove<Chain>().remove<Pierce>().remove<Bounce>();

        it.world().entity().is_a(prefab).child_of(it.entity(i).parent())
                .set<core::Position2D>(pos)
                .set<rendering::Rotation>({
                    rot.angle + 90.0f
                })
                .set<physics::Velocity2D>({
                    {right}
                }).remove<Split>().remove<Chain>().remove<Pierce>().remove<Bounce>();
    }
}
#endif //PROJECTILE_SPLIT_COLLISION_SYSTEM_H
