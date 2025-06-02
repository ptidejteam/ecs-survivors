//
// Created by laurent on 29/05/25.
//

#ifndef SPAWN_ENEMIES_AROUND_SCREEN_SYSTEM_H
#define SPAWN_ENEMIES_AROUND_SCREEN_SYSTEM_H

#include <flecs.h>
#include "modules/engine/core/components.h"
#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline bool outside_side_switch = false;
    inline void spawn_enemies_around_screen_system(flecs::iter& iter, size_t i, const Spawner &spawner,
                                                  const core::GameSettings &settings) {
        float factor = rand() % 2 - 1;
        float neg = rand() % 1 - 1;
        float randX = outside_side_switch
                          ? neg * factor * settings.windowWidth
                          : rand() % settings.windowWidth;
        float randY = outside_side_switch
                          ? rand() % settings.windowHeight
                          : neg * factor * settings.windowHeight;

        iter.world().entity().is_a(spawner.enemy_prefab).child_of(iter.entity(i))
                .set<core::Position2D>({randX, randY});

        outside_side_switch = !outside_side_switch;
    }
}
#endif //SPAWN_ENEMIES_AROUND_SCREEN_SYSTEM_H
