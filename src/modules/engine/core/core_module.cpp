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

        world.system("Remove empty tables to avoid fragmentation in collision (CHANGE TO DONTFRAGMENT WHEN FEATURE IS OUT)")
            .interval(5.0f)
            .kind(flecs::PostFrame)
            .run([world](flecs::iter& it) {
                ecs_delete_empty_tables_desc_t desc;
                desc.delete_generation = true;
                ecs_delete_empty_tables(world.c_ptr(), &desc);
                std::printf("Removing empty tables to avoid fragmentation in collision\n");
            });
    }
}
