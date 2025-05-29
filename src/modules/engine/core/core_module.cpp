//
// Created by Laurent Voisard on 12/21/2024.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "core_module.h"

#include "flecs.h"
#include "components.h"
#include "modules/engine/input/components.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/physics/physics_module.h"

#include <raymath.h>

#include "systems/destroy_entity_after_frame_system.h"
#include "systems/destroy_entity_after_time_system.h"
#include "systems/remove_empty_tables_system.h"

namespace core {
    using namespace physics;
    using namespace input;

    void CoreModule::register_components(flecs::world &world) {
        world.component<Position2D>();
        world.component<Speed>();
    }

    void CoreModule::register_systems(flecs::world &world) {
        std::cout << "Registering core systems" << std::endl;

        world.system<DestroyAfterTime>("Destroy entities after time")
                .kind(flecs::PostFrame)
                .write<DestroyAfterFrame>()
                .each(destroy_entity_after_time_system);

        world.system("Destroy entities after frame")
                .with<DestroyAfterFrame>()
                .kind(flecs::PostFrame)
                .each(destroy_entity_after_frame_system);

        world.system("Remove empty tables to avoid fragmentation in collision (CHANGE TO DONTFRAGMENT WHEN FEATURE IS OUT)")
                //.interval(0.25f)
                .kind(flecs::PostFrame)
                .run([world](flecs::iter &it) { remove_empty_tables_system(world); });
    }
}
