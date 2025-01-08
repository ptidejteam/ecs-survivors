//
// Created by Laurent Voisardnt Voisard on 12/21/2024.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "core_module.h"
#include "components.h"

#include "flecs.h"
#include "../../../cmake-build-web/_deps/raylib-src/src/raymath.h"
#include "modules/input/components.h"
#include "modules/physics/components.h"
#include "modules/physics/components.h"
#include "modules/physics/components.h"
#include "modules/physics/physics_module.h"


namespace core {
    using namespace physics;
    using namespace input;

    void CoreModule::register_components(flecs::world &world) {
        world.component<Position2D>();
    }

    void CoreModule::register_systems(flecs::world &world) {
        std::cout << "Registering core systems" << std::endl;
        world.system<const Position2D>()
                .each([](const Position2D &pos) {
                    std::printf("Position: (%f %f) \n", pos.value.x, pos.value.y);
                });

        world.system<const InputVertical, const InputHorizontal, Velocity2D>()
                .each([](const InputVertical &vertical, const InputHorizontal &horizontal,
                         Velocity2D &velocity) {
                    velocity.value.x = horizontal.value;
                    velocity.value.y = vertical.value;
                    velocity.value = Vector2Normalize(velocity.value);
                });
    }
}
