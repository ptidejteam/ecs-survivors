//
// Created by Laurent Voisard on 12/21/2024.
//

// ReSharper disable CppMemberFunctionMayBeStatic

#include <flecs.h>
#include "core/core_module.h"
#include "core/components.h"
#include "core/queries.h"

#include "core/systems/destroy_entity_after_frame_system.h"
#include "core/systems/destroy_entity_after_time_system.h"
#include "core/systems/disable_entity_on_close_system.h"
#include "core/systems/enable_entity_on_open_system.h"
#include "core/systems/remove_empty_tables_system.h"
#include "core/systems/reset_enabled_menus_system.h"
#include "core/systems/set_paused_on_entity_disable_system.h"
#include "core/systems/set_paused_on_entity_enabled_system.h"
#include "core/systems/set_time_scale_on_pause_system.h"

namespace core {
    void CoreModule::register_components(flecs::world &world) {
        world.component<Vector2>()
            .member<float>("x")
            .member<float>("y");
        world.component<Position2D>()
            .member<Vector2>("value");
        world.component<Speed>()
            .member<float>("value");
        world.component<Tag>()
            .member<std::string>("name");
        world.component<DestroyAfterTime>()
            .member<float>("time");
        world.component<DestroyAfterFrame>();
    }

        void CoreModule::register_queries(flecs::world &world) {
            queries::position_and_tag_query = world.query<Position2D, Tag>();
    }

    void CoreModule::register_systems(flecs::world &world) {
        std::cout << "Registering core systems" << std::endl;

        world.system<EnabledMenus>()
            .kind(flecs::OnStart)
            .term_at(0).singleton()
            .each(systems::reset_enabled_menus_system);

        world.observer<const Paused>()
                .event(flecs::OnSet)
                .each(systems::set_time_scale_on_pause_system);

        world.observer<EnabledMenus>()
                .term_at(0).singleton()
                .with<PauseOnEnabled>().filter()
                .event(flecs::OnAdd)
                .with(flecs::Disabled)
                .each(systems::set_paused_on_entity_disable_system);

        world.observer<EnabledMenus>()
                .term_at(0).singleton()
                .with<PauseOnEnabled>().filter()
                .event(flecs::OnRemove)
                .with(flecs::Disabled)
                .each(systems::set_paused_on_entity_enabled_system);

        world.observer()
                .event(flecs::OnAdd)
                .with<Close>()
                .each(systems::disable_entity_on_close_system);

        world.observer()
                .event(flecs::OnAdd)
                .with<Open>()
                .with(flecs::Disabled).filter()
                .each(systems::enable_entity_on_open_system);

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
