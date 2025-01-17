//
// Created by Laurent Voisardnt Voisard on 12/21/2024.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "core_module.h"

#include "flecs.h"
#include "components.h"
#include "modules/engine/input/components.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/physics/physics_module.h"

#include <raymath.h>

namespace core {
    using namespace physics;
    using namespace input;

    void CoreModule::register_components(flecs::world &world) {
        world.component<Position2D>();
        world.component<Speed>();
    }

    void CoreModule::register_systems(flecs::world &world) {
        std::cout << "Registering core systems" << std::endl;
    }
}
