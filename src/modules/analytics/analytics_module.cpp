//
// Created by laure on 3/13/2025.
//

#include "analytics_module.h"

#include "components.h"
#include "game.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/rendering/components.h"

namespace analytics {
    void AnalyticsModule::register_components(flecs::world world) {
    }

    void AnalyticsModule::register_systems(flecs::world world) {
        auto visible_query = world.query<rendering::Visible>();
        world.system()
                .rate(5)
                .run([visible_query](flecs::iter &iter) {
                    // std::printf("%d entities at %f frame time\n", visible_query.count(), iter.delta_time());
                });

        world.observer<EventBus>()
                .event<physics::CollisionDetectionPhaseCompleted>()
                .each([](EventBus &bus) {
                });
    }
} // analytics
