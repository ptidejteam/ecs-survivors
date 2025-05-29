//
// Created by laurent on 29/05/25.
//

#ifndef PROJECTILE_CHAIN_COLLIDED_SYSTEM_H
#define PROJECTILE_CHAIN_COLLIDED_SYSTEM_H
#include <flecs.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/rendering/components.h"
#include "modules/gameplay/components.h"
#include <raymath.h>

namespace gameplay {
    inline void projectile_chain_collided_system(const flecs::query<core::Tag, core::Position2D> &target_type_query, flecs::iter &it, size_t i,
                                                 Chain &chain, physics::Velocity2D &vel,
                                                 core::Position2D &pos, rendering::Rotation &rot, Attack &attack) {
        flecs::entity other = it.pair(5).second();

        if (chain.hits.contains(other.id())) {
            it.entity(i).remove<physics::CollidedWith>(other);
            return;
        }
        chain.hits.insert(other.id());
        chain.chain_count -= 1;

        float shortest_distance_sqr = 1000000;
        core::Position2D target_pos = pos;
        target_type_query.each([&](flecs::entity o, core::Tag &t, core::Position2D &o_pos) {
            if (!chain.hits.contains(o.id()) && attack.target_tag == t.name && other.id() != o.id()) {
                float d = Vector2DistanceSqr(pos.value, o_pos.value);
                if (d < shortest_distance_sqr) {
                    shortest_distance_sqr = d;
                    target_pos = o_pos;
                }
            }
        });

        if (target_pos.value == pos.value) return;

        float rad = Vector2Angle(Vector2{0, 1}, pos.value - target_pos.value);
        rot.angle = rad * RAD2DEG;
        vel.value = Vector2Normalize(target_pos.value - pos.value) * Vector2Length(vel.value);

        if (chain.chain_count < 0) {
            it.entity(i).add<core::DestroyAfterFrame>();
        }
    }
}

#endif //PROJECTILE_CHAIN_COLLIDED_SYSTEM_H
