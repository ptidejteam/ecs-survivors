//
// Created by laurent on 29/05/25.
//

#ifndef SPAWN_ENEMIES_AROUND_SCREEN_SYSTEM_H
#define SPAWN_ENEMIES_AROUND_SCREEN_SYSTEM_H

#include <flecs.h>
#include "modules/engine/core/components.h"
#include "modules/engine/physics/queries.h"
#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline bool outside_side_switch = false;

    inline void spawn_enemies_around_screen_system(flecs::iter &iter, size_t i, const Spawner &spawner,
                                                   const core::GameSettings &settings,
                                                   const rendering::TrackingCamera &camera) {
        for (int i = 0; i < 5; i++) {
            float factor = rand() % 2 - 1;
            float neg = rand() % 1 - 1;
            float randX = outside_side_switch
                              ? neg * factor * settings.windowWidth
                              : rand() % settings.windowWidth;
            randX += camera.camera.target.x - camera.camera.offset.x;
            float randY = outside_side_switch
                              ? rand() % settings.windowHeight
                              : neg * factor * settings.windowHeight;
            randY += camera.camera.target.y - camera.camera.offset.y;
            bool is_valid = true;

            physics::queries::box_collider_query.run([&](flecs::iter &it) {
                while (it.next()) {
                    if (!is_valid) break;
                    auto box = it.field<physics::BoxCollider>(0);
                    for (auto j: it) {
                        if (CheckCollisionPointRec({randX, randY}, box[j].rec)) {
                            is_valid = false;
                            break;
                        }
                    }
                }
            });

            outside_side_switch = !outside_side_switch;

            if (!is_valid) continue;

            iter.world().entity().is_a(spawner.enemy_prefab).child_of(iter.entity(i))
                    .set<core::Position2D>({randX, randY});
            break;
        }
    }
}
#endif //SPAWN_ENEMIES_AROUND_SCREEN_SYSTEM_H
