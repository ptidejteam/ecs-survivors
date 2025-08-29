//
// Created by Laurent Voisard on 1/10/2025.
//

#include "player_module.h"

#include "components.h"
#include "input/components.h"

#include <raylib.h>
#include <raymath.h>

#include "core/components.h"
#include "physics/physics_module.h"
#include "physics/components.h"
#include "systems/scale_desired_velocity_system.h"
#include "systems/translate_horizontal_input_to_desired_vel_system.h"
#include "systems/translate_vertical_input_to_desired_vel_system.h"

namespace player {
    void PlayerModule::register_components(flecs::world world) {
    }

    void PlayerModule::register_systems(flecs::world world) {
        // player movement ================================================
        world.system<const input::InputHorizontal, physics::DesiredVelocity2D>()
                .term_at(1).parent().cascade()
                .each(systems::translate_horizontal_input_to_desired_vel_system);
        
        world.system<const input::InputVertical, physics::DesiredVelocity2D>()
                .term_at(1).parent().cascade()
                .each(systems::translate_vertical_input_to_desired_vel_system);

        world.system<physics::DesiredVelocity2D, const core::Speed>()
                .each(systems::scale_desired_velocity_system);
    }
}
