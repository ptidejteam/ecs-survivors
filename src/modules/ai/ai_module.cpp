//
// Created by Laurent Voisard on 1/10/2025.
//

#include "ai_module.h"

#include <raylib.h>
#include <raymath.h>

#include "components.h"
#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

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
                .each([world](flecs::iter &it, size_t i,
                              const core::Position2D &position,
                              const core::Speed &speed,
                              physics::DesiredVelocity2D &velocity) {
                    const flecs::entity target = it.pair(3).second();
                    if (target.id() == 0) return;
                    const Vector2 dir = Vector2Normalize(target.get<core::Position2D>()->value - position.value);
                    velocity.value = dir * speed.value;
                });

        world.system<const StoppingDistance, const core::Position2D, physics::DesiredVelocity2D>(
                    "Stop when arrived at distance of target")
                .with<Target>(flecs::Wildcard)
                .each([world](flecs::iter &it, size_t i,
                              const StoppingDistance &distance,
                              const core::Position2D &pos,
                              physics::DesiredVelocity2D &velocity) {
                    const flecs::entity target = it.pair(3).second();
                    if (target.id() == 0) return;
                    const Vector2 ab = target.get<core::Position2D>()->value - pos.value;

                    // using the squared length is faster computationally
                    const float distSquared = Vector2LengthSqr(ab);

                    // square the distance
                    if (distSquared < distance.value * distance.value) {
                        velocity.value = {0, 0};
                    }
                });
    }
}
