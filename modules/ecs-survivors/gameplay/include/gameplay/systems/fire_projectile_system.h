//
// Created by laurent on 29/05/25.
//

#ifndef FIRE_PROJECTILE_SYSTEM_H
#define FIRE_PROJECTILE_SYSTEM_H

#include <flecs.h>

#include <raylib.h>
#include <raymath.h>
#include "core/components.h"
#include "physics/components.h"
#include "rendering/components.h"
#include "../components.h"
#include "core/queries.h"

namespace gameplay::systems {
    inline void fire_projectile_system(flecs::iter &iter, size_t index, core::Position2D &pos, Attack &attack,
                                       core::Speed &speed, MultiProj *multi_proj) {
        float shortest_distance_sqr = 1000000;
        core::Position2D target_pos = pos;
        core::queries::position_and_tag_query.each([&](core::Position2D &o_pos, core::Tag &t) {
            if (attack.target_tag != t.name) return;
            float d = Vector2DistanceSqr(pos.value, o_pos.value);
            if (d > shortest_distance_sqr) return;
            shortest_distance_sqr = d;
            target_pos = o_pos;
        });

        if (target_pos.value == pos.value) return;

        float rot = Vector2Angle(Vector2{0, 1}, pos.value - target_pos.value) * RAD2DEG;

        int proj_count = multi_proj ? multi_proj->projectile_count : 1;
        float spread_angle = multi_proj ? multi_proj->spread_angle : 0.0f;

        float offset = proj_count % 2 == 0 ? spread_angle / proj_count / 2 : 0;


        auto prefab = iter.world().lookup(attack.attack_prefab_name.c_str());

        for (int i = -proj_count / 2; i < (proj_count + 1) / 2; i++) {
            float angle = i * (spread_angle / proj_count) + offset;
            iter.world().entity().is_a(prefab).child_of(iter.entity(index))
                    .set<core::Position2D>({pos.value})
                    .set<rendering::Rotation>({rot + angle})
                    .set<core::Speed>({150})
                    .set<physics::Velocity2D>({
                        Vector2Rotate(Vector2Normalize(target_pos.value - pos.value) * speed.value, angle * DEG2RAD)
                    });
        }
        iter.entity(index).remove<CooldownCompleted>();
    }
}
#endif //FIRE_PROJECTILE_SYSTEM_H
