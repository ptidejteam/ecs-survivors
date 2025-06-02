//
// Created by laurent on 30/05/25.
//

#ifndef DEBUG_CLOSEST_ENEMY_TO_PLAYER_SYSTEM_H
#define DEBUG_CLOSEST_ENEMY_TO_PLAYER_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "modules/engine/core/components.h"
#include "modules/engine/core/queries.h"

namespace debug::systems {
    inline void debug_closest_enemy_to_player_system(flecs::iter &iter) {
        auto player = iter.world().lookup("player");
        auto pos = player.get<core::Position2D>();
        float shortest_distance_sqr = 10000000;
        core::Position2D target_pos{pos->value};
        core::queries::position_and_tag_query.each([&](const core::Position2D &other_pos, const core::Tag &tag) {
            if ("enemy" != tag.name) return;
            float d = Vector2DistanceSqr(pos->value, other_pos.value);
            if (d > shortest_distance_sqr) return;
            shortest_distance_sqr = d;
            target_pos = other_pos;
        });

        if (target_pos.value == pos->value) return;

        DrawLineEx(Vector2{pos->value.x, pos->value.y}, Vector2{target_pos.value.x, target_pos.value.y}, 1,
                   GREEN);
    }
}
#endif //DEBUG_CLOSEST_ENEMY_TO_PLAYER_SYSTEM_H
