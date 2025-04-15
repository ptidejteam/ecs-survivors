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
        world.system<const Target, const core::Position2D, const core::Speed, physics::DesiredVelocity2D>("Follow Target")
                .with<FollowTarget>()
                .each([world](const Target &target,
                              const core::Position2D &position,
                              const core::Speed &speed,
                              physics::DesiredVelocity2D &velocity) {
                    const flecs::entity t =world.lookup(target.name.c_str());
                    if (t.id() == 0) return;
                    const Vector2 dir = Vector2Normalize(t.get<core::Position2D>()->value - position.value);
                    velocity.value = dir * speed.value;
                });

        world.system<const StoppingDistance, const Target, const core::Position2D, physics::DesiredVelocity2D>("Stop when arrived at distance of target")
                .each([world](const StoppingDistance &distance,
                              const Target &target,
                              const core::Position2D &pos,
                              physics::DesiredVelocity2D &velocity) {
                    const flecs::entity t =world.lookup(target.name.c_str());
                    if (t.id() == 0) return;
                    const Vector2 ab = t.get<core::Position2D>()->value - pos.value;

                    // using the squared length is faster computationally
                    const float distSquared = Vector2LengthSqr(ab);

                    // square the distance
                    if (distSquared < distance.value * distance.value) {
                        velocity.value = {0, 0};
                    }
                });
    }
}
