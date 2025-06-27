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
        for (int i = 0; i < 1; i++) {
            float factor = rand() % 2 - 1;
            float neg = rand() % 1 - 1;
            float randX = outside_side_switch
                              ? neg * factor * (settings.window_width + 200)
                              : rand() % (settings.window_width + 200);
            randX += camera.camera.target.x - camera.camera.offset.x - 100;
            float randY = outside_side_switch
                              ? rand() % (settings.window_height + 200)
                              : neg * factor * (settings.window_height + 200);
            randY += camera.camera.target.y - camera.camera.offset.y - 100;
            bool is_valid = true;

            physics::queries::box_collider_query.run([&](flecs::iter &it) {
                while (it.next()) {
                    if (!is_valid) break;
                    auto pos = it.field<core::Position2D>(0);
                    auto box = it.field<physics::Collider>(1);
                    for (auto j: it) {
                        const Rectangle rec = {pos[j].value.x + box[j].bounds.x, pos[j].value.y + box[j].bounds.y, box[j].bounds.width, box[j].bounds.height};
                        if (CheckCollisionPointRec({randX, randY}, rec)) {
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
