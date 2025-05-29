//
// Created by laure on 3/12/2025.
//

#include "gameplay_module.h"
#include "components.h"
#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"
#include <raymath.h>
#include <set>

#include "modules/engine/rendering/components.h"
#include <unordered_set>

#include "modules/engine/physics/physics_module.h"
#include "modules/engine/rendering/gui/gui_module.h"
#include "systems/add_chain_system.h"
#include "systems/add_multiproj_system.h"
#include "systems/add_pierce_system.h"
#include "systems/add_split_system.h"
#include "systems/deal_damage_on_collision_system.h"
#include "systems/decrement_chain_system.h"
#include "systems/decrement_multiproj_system.h"
#include "systems/decrement_pierce_system.h"
#include "systems/fire_projectile_system.h"
#include "systems/increment_chain_system.h"
#include "systems/increment_multiproj_system.h"
#include "systems/increment_pierce_system.h"
#include "systems/projectile_chain_collided_system.h"
#include "systems/projectile_no_effect_collided_system.h"
#include "systems/projectile_pierce_collided_system.h"
#include "systems/projectile_split_collision_system.h"
#include "systems/regen_health_system.h"
#include "systems/remove_chain_system.h"
#include "systems/remove_multproj_system.h"
#include "systems/remove_pierce_system.h"
#include "systems/remove_split_system.h"
#include "systems/restart_cooldown_system.h"
#include "systems/spawn_enemies_around_screen_system.h"
#include "systems/take_damage_system.h"
#include "systems/update_cooldown_system.h"

namespace gameplay {
    void GameplayModule::register_components(flecs::world world) {
        world.component<Spawner>();
    }

    void GameplayModule::register_systems(flecs::world world) {
        m_spawner_tick = world.timer().interval(SPAWNER_INTERVAL);
        world.system<const Spawner, const core::GameSettings>("Spawn Enemies")
                .tick_source(m_spawner_tick)
                .term_at(1).singleton()
                .each([world](flecs::entity self, const Spawner &spawner, const core::GameSettings &settings) {
                    spawn_enemies_around_screen_system(world, self, spawner, settings);
                });

        world.system<Cooldown>("Update Cooldown")
                .without<CooldownCompleted>()
                .each(update_cooldown_system);

        world.observer("Restart Cooldown")
                .with<CooldownCompleted>()
                .event(flecs::OnRemove)
                .each(restart_cooldown_system);

        auto target_type_query = world.query_builder<core::Tag, core::Position2D>().build();

        world.system<core::Position2D, Attack, core::Speed, MultiProj *>("Fire Projectile")
                .with<Projectile>()
                .with<CooldownCompleted>()
                .write<CooldownCompleted>()
                .term_at(0).parent()
                .each([world, target_type_query](flecs::entity e, core::Position2D &pos, Attack &attack,
                                                 core::Speed &speed, MultiProj *multi_proj) {
                    fire_projectile_system(world, target_type_query, e, pos, attack, speed, multi_proj);
                });

        world.system("no pierce or chain")
                .with<Projectile>()
                .without<Pierce>().without<Chain>()
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each(project_no_effect_collided_system);

        world.system<Pierce>("apply pierce mod")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each(projectile_pierce_collided_system);

        world.system<Chain, physics::Velocity2D, core::Position2D, rendering::Rotation, Attack>("apply chain mod")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each([target_type_query](flecs::iter &it, size_t i, Chain &chain, physics::Velocity2D &vel,
                                          core::Position2D &pos, rendering::Rotation &rot, Attack &attack) {
                    projectile_chain_collided_system(target_type_query, it, i, chain, vel, pos, rot, attack);
                });


        world.system<Split, physics::Velocity2D, core::Position2D, rendering::Rotation, Attack>("apply split mod")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each([world](flecs::iter &it, size_t i, Split &split, physics::Velocity2D &vel, core::Position2D &pos,
                              rendering::Rotation &rot, Attack &attack) {
                    projectile_split_collision_system(world, it, i, split, vel, pos, rot, attack);
                });

        world.system<Damage>("collision detected, deal damage to target")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .write<TakeDamage>()
                .kind(flecs::PostUpdate)
                .each(deal_damage_on_collision_system);


        world.system<Health, TakeDamage>("take damage")
                .kind(flecs::PostUpdate)
                .each(take_damage_system);

        world.system<Health, RegenHealth>("regen health")
                .kind(flecs::PostUpdate)
                .each(regen_health_system);

        add_multiproj = world.system("add multi proj")
                .kind(0)
                .with<Attack>()
                .without<MultiProj>()
                .with(flecs::ChildOf, "player")
                .immediate()
                .each(add_multiproj_system);

        remove_multiproj = world.system("remove multi proj")
                .kind(0)
                .with<Attack>()
                .with<MultiProj>()
                .with(flecs::ChildOf, "player")
                .immediate()
                .each(remove_multiproj_system);

        add_pierce = world.system("add pierce")
                .kind(0)
                .with<Projectile>()
                .without<Pierce>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    add_pierce_system(world, e);
                });

        remove_pierce = world.system("remove pierce")
                .kind(0)
                .with<Projectile>()
                .with<Pierce>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    remove_pierce_system(world, e);
                });

        add_chain = world.system("add chain")
                .kind(0)
                .with<Projectile>()
                .without<Chain>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    add_chain_system(world, e);
                });

        remove_chain = world.system("remove chain")
                .kind(0)
                .with<Projectile>()
                .with<Chain>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    remove_chain_system(world, e);
                });

        add_split = world.system("add split")
                .kind(0)
                .with<Projectile>()
                .without<Split>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    add_split_system(world, e);
                });

        remove_split = world.system("remove split")
                .kind(0)
                .with<Projectile>()
                .with<Split>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    remove_split_system(world, e);
                });

        add_proj = world.system<MultiProj>("+1 proj")
                .kind(0)
                .with<Attack>()
                .with(flecs::ChildOf, "player")
                .each(increment_multiproj_system);

        remove_proj = world.system<MultiProj>("-1 proj")
                .kind(0)
                .with<Attack>()
                .with(flecs::ChildOf, "player")
                .each(decrement_multiproj_system);

        add_pierce_amt = world.system<Pierce>("+1 Pierce")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each(increment_pierce_system);

        remove_pierce_amt = world.system<Pierce>("-1 Pierce")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each(decrement_pierce_system);

        add_chain_amt = world.system<Chain>("+1 Chain")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each(increment_chain_system);

        remove_chain_amt = world.system<Chain>("-1 Chain")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each(decrement_chain_system);
    }

    void GameplayModule::register_entities(flecs::world world) {
        auto dropdown = world.entity("more_dropdown").child_of(rendering::gui::GUIModule::menu_bar)
                .set<rendering::gui::MenuBarTab>({"Gameplay Tools", 25});

        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Add Multi Proj", add_multiproj, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Remove Multi Proj", remove_multiproj, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"+1 Proj", add_proj, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"-1 Proj", remove_proj, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>(
                    {"Add Pierce", add_pierce, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Remove Pierce", remove_pierce, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "+1 Pierce", add_pierce_amt, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "-1 Pierce", remove_pierce_amt, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Add Chain", add_chain, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Remove Chain", remove_chain, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "+1 Chain", add_chain_amt, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "-1 Chain", remove_chain_amt, rendering::gui::MenuBarTabItemType::RUN
                });
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Add Split", add_split, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Remove Split", remove_split, rendering::gui::MenuBarTabItemType::RUN
                });
    }
}
