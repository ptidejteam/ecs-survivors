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

#include "queries.h"
#include "systems/destroy_entity_after_frame_system.h"
#include "systems/destroy_entity_after_time_system.h"
#include "systems/remove_empty_tables_system.h"

namespace core {
    void CoreModule::register_components(flecs::world &world) {
        world.component<Position2D>();
        world.component<Speed>();
        world.component<GameSettings>();
        world.component<Tag>();
        world.component<DestroyAfterTime>();
        world.component<DestroyAfterFrame>();
    }

    void CoreModule::register_queries(flecs::world &world) {
        queries::position_and_tag_query = world.query<Position2D, Tag>();
    }

    void CoreModule::register_systems(flecs::world &world) {
        std::cout << "Registering core systems" << std::endl;

        world.observer<const Paused>()
                .event(flecs::OnSet)
                .each([](flecs::iter &it, size_t i, const Paused &paused) {
                    it.world().set_time_scale(!paused.paused);
                });

        world.observer<EnabledMenus>()
                .term_at(0).singleton()
                .with<PauseOnEnabled>().filter()
                .event(flecs::OnAdd)
                .with(flecs::Disabled)
                .each([](flecs::iter &it, size_t i, EnabledMenus &enabled) {
                    enabled.count --;
                    if (enabled.count == 0)
                        it.world().set<Paused>({false});
                });

        world.observer<EnabledMenus>()
                .term_at(0).singleton()
                .with<PauseOnEnabled>().filter()
                .event(flecs::OnRemove)
                .with(flecs::Disabled)
                .each([](flecs::iter &it, size_t i, EnabledMenus &enabled) {
                    enabled.count ++;
                    it.world().set<Paused>({true});
                });


        world.system<DestroyAfterTime>("Destroy entities after time")
                .kind(flecs::PostFrame)
                .write<DestroyAfterFrame>()
                .multi_threaded()
                .each(systems::destroy_entity_after_time_system);

        world.system("Destroy entities after frame")
                .with<DestroyAfterFrame>()
                .kind(flecs::PostFrame)
                .multi_threaded()
                .each(systems::destroy_entity_after_frame_system);

        world.system(
                    "Remove empty tables to avoid fragmentation in collision (CHANGE TO DONTFRAGMENT WHEN FEATURE IS OUT)")
                //.interval(0.25f)
                .kind(flecs::PostFrame)
                .run([world](flecs::iter &it) { systems::remove_empty_tables_system(world); });
    }
}
