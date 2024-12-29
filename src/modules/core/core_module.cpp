//
// Created by Laurent Voisardnt Voisard on 12/21/2024.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "core_module.h"
#include "components.h"

#include "flecs.h"

namespace core {
    void CoreModule::register_components(flecs::world& world) {
        world.component<Position2D>();
    }

    void CoreModule::register_systems(flecs::world& world)  {
        std::cout << "Registering core systems" << std::endl;
        world.system<const Position2D>()
        .each([](const Position2D &pos) {
            std::printf("Position: (%f %f) \n", pos.value.x, pos.value.y);
        });
    }
}