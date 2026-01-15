//
// Created by Laurent Voisard on 1/10/2025.
//

#include "ai/ai_module.h"

#include "ai/components.h"
#include "core/components.h"
#include "physics/components.h"
#include "ai/systems/follow_target_system.h"
#include "ai/systems/stop_when_arrived_at_target_system.h"

namespace ai {
    void AIModule::register_components(flecs::world world) {
        world.component<Target>();
        world.component<FollowTarget>();
        world.component<StoppingDistance>();
    }

    void AIModule::register_systems(flecs::world world) {
        world.system<const core::Position2D, const core::Speed, physics::DesiredVelocity2D>("Follow Target")
                .with<Target>(flecs::Wildcard)
                .with<FollowTarget>()
                .each(systems::follow_target_system);

        world.system<const StoppingDistance, const core::Position2D, physics::DesiredVelocity2D>(
                    "Stop when arrived at distance of target")
                .with<Target>(flecs::Wildcard)
                .each(systems::stop_when_arrived_at_target_system);
    }
}
