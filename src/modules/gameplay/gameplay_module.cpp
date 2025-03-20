//
// Created by laure on 3/12/2025.
//

#include "gameplay_module.h"

#include "components.h"
#include "modules/engine/core/components.h"

namespace gameplay {
    void GameplayModule::register_components(flecs::world world) {
        world.component<Spawner>();
        m_spawner_tick = world.timer().interval(SPAWNER_INTERVAL);
    }

    bool outside_side_switch = false;
    int count = 0;

    void GameplayModule::register_systems(flecs::world world) {
        world.system<const Spawner, const core::GameSettings>("Spawn Enemies")
                .tick_source(m_spawner_tick)
                .term_at(1).singleton()
                .each([&,world](flecs::entity self, const Spawner &spawner, const core::GameSettings &settings) {
                    const flecs::entity e = world.lookup(spawner.enemy_prefab_name.c_str());
                    if (count > 20000) return;

                    if (0 == e) return;

                    for (int i = 0; i < 3; i++) {
                        float factor = rand() % 2 - 1;
                        float neg = rand() % 1 - 1;
                        float randX = outside_side_switch
                                          ? neg * factor * settings.windowWidth
                                          : rand() % settings.windowWidth;
                        float randY = outside_side_switch
                                          ? rand() % settings.windowHeight
                                          : neg * factor * settings.windowHeight;

                        world.entity().is_a(e).child_of(self)
                                .set<core::Position2D>({randX, randY});

                        outside_side_switch = !outside_side_switch;
                        count++;
                    }
                });

        // world.system<const Spawner, const core::GameSettings>("delete enemies")
        //         .tick_source(m_spawner_tick)
        //         .term_at(1).singleton()
        //         .each([&,world](flecs::entity self, const Spawner &spawner, const core::GameSettings &settings) {
        //             const flecs::entity e = world.lookup(spawner.enemy_prefab_name.c_str());
        //             if (count > 20000) return;
        //
        //             if (0 == e) return;
        //
        //             for (int i = 0; i < 3; i++) {
        //                 float factor = rand() % 2 - 1;
        //                 float neg = rand() % 1 - 1;
        //                 float randX = outside_side_switch
        //                                   ? neg * factor * settings.windowWidth
        //                                   : rand() % settings.windowWidth;
        //                 float randY = outside_side_switch
        //                                   ? rand() % settings.windowHeight
        //                                   : neg * factor * settings.windowHeight;
        //
        //                 world.entity().is_a(e).child_of(self)
        //                         .set<core::Position2D>({randX, randY});
        //
        //                 outside_side_switch = !outside_side_switch;
        //                 count++;
        //             }
        //         });
    }
}
