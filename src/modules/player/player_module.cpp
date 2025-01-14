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
        world.system<const input::InputHorizontal, MovementInputDirection>()
                .term_at(1).parent().cascade()
                .each([](flecs::entity e,
                         const input::InputHorizontal &horizontal,
                         MovementInputDirection &inputDir) {
                    inputDir.dir.x = horizontal.value;
                });
        world.system<const input::InputVertical, MovementInputDirection>()
                .term_at(1).parent().cascade()
                .each([](flecs::entity e,
                         const input::InputVertical &vertical,
                         MovementInputDirection &inputDir) {
                    inputDir.dir.y = vertical.value;
                });

        world.system<MovementInputDirection>()
                .each([](flecs::entity e,
                         MovementInputDirection &inputDir) {
                    inputDir.dir = Vector2Normalize(inputDir.dir);
                });

        world.system<physics::DesiredVelocity2D, const core::Speed, const MovementInputDirection>()
                .each([](flecs::entity e,
                         physics::DesiredVelocity2D &velocity,
                         const core::Speed &speed,
                         const MovementInputDirection &inputDir) {
                    velocity.value = inputDir.dir * speed.value;
                });
    }
}
