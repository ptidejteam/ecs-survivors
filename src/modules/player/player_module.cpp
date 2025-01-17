//
// Created by laure on 1/10/2025.
//

#include "player_module.h"

#include "components.h"
#include "modules/engine/input/components.h"

#include <raylib.h>
#include <raymath.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/physics_module.h"
#include "modules/engine/physics/components.h"

namespace player {
    void PlayerModule::register_components(flecs::world world) {
        world.component<PlayerControlled>();
    }

    void PlayerModule::register_systems(flecs::world world) {
        // player movement ================================================
        world.system<const input::InputHorizontal, physics::DesiredVelocity2D>()
                .term_at(1).parent().cascade()
                .each([](const input::InputHorizontal &horizontal,
                         physics::DesiredVelocity2D &desired_vel) {
                    desired_vel.value.x = horizontal.value;
                });
        world.system<const input::InputVertical, physics::DesiredVelocity2D>()
                .term_at(1).parent().cascade()
                .each([](const input::InputVertical &vertical,
                         physics::DesiredVelocity2D &desired_vel) {
                    desired_vel.value.y = vertical.value;
                });

        world.system<physics::DesiredVelocity2D, const core::Speed>()
                .each([](physics::DesiredVelocity2D &velocity,
                         const core::Speed &speed) {
                    velocity.value = Vector2Normalize(velocity.value) * speed.value;
                });
    }
}
