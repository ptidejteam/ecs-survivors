//
// Created by laure on 1/10/2025.
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

        world.system<const Target, const core::Position2D, const core::Speed>()
                .with<FollowTarget>()
                .each([world](flecs::entity self, const Target &target, const core::Position2D &position,
                              const core::Speed &speed) {
                    const flecs::entity e = world.entity(target.name.c_str());
                    if (e != world.lookup(target.name.c_str())) return;
                    const Vector2 dir = Vector2Normalize(e.get<core::Position2D>()->value - position.value);
                    self.set<physics::DesiredVelocity2D>({dir * speed.value});
                });

        world.system<const StoppingDistance, const Target, const core::Position2D>()
                .each([world](flecs::entity self, const StoppingDistance &distance, const Target &target,
                              const core::Position2D &pos) {
                    const flecs::entity e = world.entity(target.name.c_str());
                    if (e != world.lookup(target.name.c_str())) return;
                    const Vector2 ab = e.get<core::Position2D>()->value - pos.value;

                    // using the squared length is faster computationally
                    const float dist = Vector2LengthSqr(ab);

                    // square the distance
                    if (dist < distance.value * distance.value) {
                        self.set<physics::DesiredVelocity2D>({0, 0});
                    }
                });
    }
}
