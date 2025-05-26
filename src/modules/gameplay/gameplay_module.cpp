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

namespace gameplay {
    void GameplayModule::register_components(flecs::world world) {
        world.component<Spawner>();
        m_spawner_tick = world.timer().interval(SPAWNER_INTERVAL);
    }

    bool outside_side_switch = false;

    void GameplayModule::register_systems(flecs::world world) {
        world.system<const Spawner, const core::GameSettings>("Spawn Enemies")
                .tick_source(m_spawner_tick)
                .term_at(1).singleton()
                .each([&,world](flecs::entity self, const Spawner &spawner, const core::GameSettings &settings) {
                    const flecs::entity e = world.lookup(spawner.enemy_prefab_name.c_str());
                    if (world.query<physics::Collider>().count() > 5000) return;

                    if (0 == e) return;

                    float factor = rand() % 2 - 1;
                    float neg = rand() % 1 - 1;
                    float randX = outside_side_switch
                                      ? neg * factor * settings.windowWidth
                                      : rand() % settings.windowWidth;
                    float randY = outside_side_switch
                                      ? rand() % settings.windowHeight
                                      : neg * factor * settings.windowHeight;

                    world.entity().is_a(e).child_of(self)
                            .set<core::Position2D>({randX, randY});

                    outside_side_switch = !outside_side_switch;
                });

        world.system<Cooldown>("Update Cooldown")
                .without<CooldownCompleted>()
                .each([](flecs::iter &it, size_t i, Cooldown &cooldown) {
                    cooldown.elapsed_time += it.delta_time();
                    if (cooldown.elapsed_time > cooldown.value)
                        it.entity(i).add<CooldownCompleted>();
                });

        world.observer("Restart Cooldown")
                .with<CooldownCompleted>()
                .event(flecs::OnRemove)
                .each([](flecs::entity e) {
                    if (e.has<Cooldown>())
                        e.set<Cooldown>({e.get<Cooldown>()->value, 0.0f});
                });

        auto target_type_query = world.query_builder<core::Tag, core::Position2D>().build();

        world.system<core::Position2D, core::Attack, core::Speed, MultiProj *>("Fire Projectile")
                .with<Projectile>()
                .with<CooldownCompleted>()
                .write<CooldownCompleted>()
                .term_at(0).parent()
                .each([world, target_type_query](flecs::entity e, core::Position2D &pos, core::Attack &attack,
                                                 core::Speed &speed, MultiProj *multi_proj) {
                    float shortest_distance_sqr = 1000000;
                    core::Position2D target_pos = pos;
                    target_type_query.each([&](flecs::entity o, core::Tag &t, core::Position2D &o_pos) {
                        if (attack.target_tag != t.name) return;
                        float d = Vector2DistanceSqr(pos.value, o_pos.value);
                        if (d > shortest_distance_sqr) return;
                        shortest_distance_sqr = d;
                        target_pos = o_pos;
                    });

                    if (target_pos.value == pos.value) return;

                    float rot = Vector2Angle(Vector2{0, 1}, pos.value - target_pos.value) * RAD2DEG;

                    int proj_count = multi_proj ? multi_proj->projectile_count : 1;
                    float spread_angle = multi_proj ? multi_proj->spread_angle : 0.0f;

                    float offset = proj_count % 2 == 0 ? spread_angle / proj_count / 2 : 0;


                    auto prefab = world.lookup(attack.attack_prefab_name.c_str());

                    for (int i = -proj_count / 2; i < (proj_count + 1) / 2; i++) {
                        world.entity().is_a(prefab).child_of(e)
                                .set<core::Position2D>({pos.value + Vector2{0, 0} * i})
                                .set<rendering::Rotation>({
                                    rot + ((i * (spread_angle / proj_count) + offset))
                                })
                                .set<core::Speed>({150})
                                .set<physics::Velocity2D>({
                                    Vector2Rotate(Vector2Normalize(target_pos.value - pos.value) * speed.value,
                                                  (i * (spread_angle / proj_count) + offset) * DEG2RAD)
                                });
                    }
                    e.remove<CooldownCompleted>();
                });

        world.system("no pierce or chain")
                .with<Projectile>()
                .without<Pierce>().without<Chain>()
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each([](flecs::iter &it, size_t i) {
                    it.entity(i).add<core::DestroyAfterFrame>();
                });

        world.system<Pierce>("apply pierce mod")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each([](flecs::iter &it, size_t i, Pierce &pierce) {
                    flecs::entity other = it.pair(1).second();
                    if (pierce.hits.contains(other.id())) {
                        it.entity(i).remove<physics::CollidedWith>(other);
                        return;
                    }
                    pierce.hits.insert(other.id());
                    pierce.pierce_count -= 1;
                    if (pierce.pierce_count < 0) {
                        it.entity(i).add<core::DestroyAfterFrame>();
                    }
                });

        world.system<Chain, physics::Velocity2D, core::Position2D, rendering::Rotation, core::Attack>("apply chain mod")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each([target_type_query](flecs::iter &it, size_t i, Chain &chain, physics::Velocity2D &vel,
                                          core::Position2D &pos, rendering::Rotation &rot, core::Attack &attack) {
                    flecs::entity other = it.pair(5).second();

                    if (chain.hits.contains(other.id())) {
                        it.entity(i).remove<physics::CollidedWith>(other);
                        return;
                    }
                    chain.hits.insert(other.id());
                    chain.chain_count -= 1;

                    float shortest_distance_sqr = 1000000;
                    core::Position2D target_pos = pos;
                    target_type_query.each([&](flecs::entity o, core::Tag &t, core::Position2D &o_pos) {
                        if (!chain.hits.contains(o.id()) && attack.target_tag == t.name && other.id() != o.id()) {
                            float d = Vector2DistanceSqr(pos.value, o_pos.value);
                            if (d < shortest_distance_sqr) {
                                shortest_distance_sqr = d;
                                target_pos = o_pos;
                            }
                        }
                    });

                    if (target_pos.value == pos.value) return;

                    float rad = Vector2Angle(Vector2{0, 1}, pos.value - target_pos.value);
                    rot.angle = rad * RAD2DEG;
                    vel.value = Vector2Normalize(target_pos.value - pos.value) * Vector2Length(vel.value);

                    if (chain.chain_count < 0) {
                        it.entity(i).add<core::DestroyAfterFrame>();
                    }
                });


        world.system<Split, physics::Velocity2D, core::Position2D, rendering::Rotation, core::Attack>("apply split mod")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .immediate()
                .each([world](flecs::iter &it, size_t i, Split &split, physics::Velocity2D &vel, core::Position2D &pos,
                              rendering::Rotation &rot, core::Attack &attack) {
                    flecs::entity other = it.pair(5).second();

                    if (split.hits.contains(other.id())) {
                        it.entity(i).remove<physics::CollidedWith>(other);
                        return;
                    }
                    split.hits.insert(other.id());

                    Vector2 left = Vector2Rotate(vel.value, -90 * DEG2RAD);
                    Vector2 right = Vector2Rotate(vel.value, 90 * DEG2RAD);

                    auto prefab = world.lookup(attack.attack_prefab_name.c_str());
                    world.entity().is_a(prefab).child_of(it.entity(i).parent())
                            .set<core::Position2D>(pos)
                            .set<rendering::Rotation>({
                                rot.angle - 90.0f
                            })
                            .set<physics::Velocity2D>({
                                {left}
                            }).remove<Split>().remove<Chain>().remove<Pierce>();

                    world.entity().is_a(prefab).child_of(it.entity(i).parent())
                            .set<core::Position2D>(pos)
                            .set<rendering::Rotation>({
                                rot.angle + 90.0f
                            })
                            .set<physics::Velocity2D>({
                                {right}
                            }).remove<Split>().remove<Chain>().remove<Pierce>();
                });

        world.system<core::Damage>("collision detected, deal damage to target")
                .with<physics::CollidedWith>(flecs::Wildcard)
                .write<TakeDamage>()
                .kind(flecs::PostUpdate)
                .each([](flecs::iter &it, size_t i, core::Damage &dmg) {
                    flecs::entity other = it.pair(1).second();

                    if (other.has<TakeDamage>() && !other.has<core::Health>()) return;
                    other.set<TakeDamage>({dmg.value});
                });


        world.system<core::Health, TakeDamage>("take damage")
                .kind(flecs::PostUpdate)
                .each([](flecs::entity e, core::Health &health, TakeDamage &dmg) {
                    health.value -= dmg.damage;
                    if (health.value <= 0)
                        e.add<core::DestroyAfterFrame>();
                    e.remove<TakeDamage>();
                });

        world.system<core::Health, RegenHealth>("regen health")
                .kind(flecs::PostUpdate)
                .each([](flecs::iter &it, size_t i, core::Health &health, RegenHealth &regen) {
                    health.value = std::min(health.value + regen.rate * it.delta_time(), health.max);
                });

        add_multiproj = world.system("add multi proj")
                .kind(0)
                .with<core::Attack>()
                .without<MultiProj>()
                .with(flecs::ChildOf, "player")
                .immediate()
                .each([](flecs::entity e) {
                    std::cout << "add multiproj" << std::endl;
                    e.set<MultiProj>({2, 15, 150.f, 30.f});
                });

        remove_multiproj = world.system("remove multi proj")
                .kind(0)
                .with<core::Attack>()
                .with<MultiProj>()
                .with(flecs::ChildOf, "player")
                .immediate()
                .each([](flecs::entity e) {
                    std::cout << "remove muti proj" << std::endl;
                    e.remove<MultiProj>();
                });

        add_pierce = world.system("add pierce")
                .kind(0)
                .with<Projectile>()
                .without<Pierce>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    e.remove<Chain>();
                    std::cout << "add pierce" << std::endl;
                    e.set<Pierce>({1, std::unordered_set<int>()});
                    ecs_delete_empty_tables_desc_t desc;
                    desc.delete_generation = true;
                    ecs_delete_empty_tables(world.c_ptr(), &desc);
                });

        remove_pierce = world.system("remove pierce")
                .kind(0)
                .with<Projectile>()
                .with<Pierce>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    std::cout << "remove pierce" << std::endl;
                    e.remove<Pierce>();
                    ecs_delete_empty_tables_desc_t desc;
                    desc.delete_generation = true;
                    ecs_delete_empty_tables(world.c_ptr(), &desc);
                });

        add_chain = world.system("add chain")
                .kind(0)
                .with<Projectile>()
                .without<Chain>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    e.remove<Pierce>();
                    std::cout << "add chain" << std::endl;
                    e.set<Chain>({1, std::unordered_set<int>()});
                    ecs_delete_empty_tables_desc_t desc;
                    desc.delete_generation = true;
                    ecs_delete_empty_tables(world.c_ptr(), &desc);
                });

        remove_chain = world.system("remove chain")
                .kind(0)
                .with<Projectile>()
                .with<Chain>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    std::cout << "remove chain" << std::endl;
                    e.remove<Chain>();
                    ecs_delete_empty_tables_desc_t desc;
                    desc.delete_generation = true;
                    ecs_delete_empty_tables(world.c_ptr(), &desc);
                });

        add_split = world.system("add split")
                .kind(0)
                .with<Projectile>()
                .without<Split>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    std::cout << "add split" << std::endl;
                    e.set<Split>({std::unordered_set<int>()});
                    ecs_delete_empty_tables_desc_t desc;
                    desc.delete_generation = true;
                    ecs_delete_empty_tables(world.c_ptr(), &desc);
                });

        remove_split = world.system("remove split")
                .kind(0)
                .with<Projectile>()
                .with<Split>()
                .with(flecs::Prefab)
                .immediate()
                .each([world](flecs::entity e) {
                    std::cout << "remove split" << std::endl;
                    e.remove<Split>();
                    ecs_delete_empty_tables_desc_t desc;
                    desc.delete_generation = true;
                    ecs_delete_empty_tables(world.c_ptr(), &desc);
                });

        add_proj = world.system<MultiProj>("+1 proj")
                .kind(0)
                .with<core::Attack>()
                .with(flecs::ChildOf, "player")
                .each([](MultiProj &p) {
                    std::cout << "+1 proj" << std::endl;
                    p.projectile_count += 1;
                    p.spread_angle = std::min(p.spread_angle + 15.0f, p.max_spread);
                });

        remove_proj = world.system<MultiProj>("-1 proj")
                .kind(0)
                .with<core::Attack>()
                .with(flecs::ChildOf, "player")
                .each([](MultiProj &p) {
                    std::cout << "-1 proj" << std::endl;
                    p.projectile_count = std::max(0, p.projectile_count - 1);
                    p.spread_angle = std::max(p.spread_angle - 15.0f, p.min_spread);
                });

        add_pierce_amt = world.system<Pierce>("+1 Pierce")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each([](Pierce &p) {
                    std::cout << "+1 pierce" << std::endl;
                    p.pierce_count += 1;
                });

        remove_pierce_amt = world.system<Pierce>("-1 Pierce")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each([](Pierce &p) {
                    std::cout << "-1 pierce" << std::endl;
                    p.pierce_count = std::max(0, p.pierce_count - 1);
                });

        add_chain_amt = world.system<Chain>("+1 Chain")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each([](Chain &c) {
                    std::cout << "+1 chain" << std::endl;
                    c.chain_count += 1;
                });

        remove_chain_amt = world.system<Chain>("-1 Chain")
                .kind(0)
                .with<Projectile>()
                .with(flecs::Prefab)
                .each([](Chain &c) {
                    std::cout << "-1 chain" << std::endl;
                    c.chain_count = std::max(0, c.chain_count - 1);
                });
    }

    void GameplayModule::register_entities(flecs::world world) {
        auto dropdown = world.entity("more_dropdown").child_of(rendering::gui::GUIModule::menu_bar)
                .set<rendering::gui::MenuBarTab>({"Gameplay Tools", 25});

        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Add Multi Proj", add_multiproj, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Remove Multi Proj", remove_multiproj, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"+1 Proj", add_proj, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"-1 Proj", remove_proj, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Add Pierce", add_pierce, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Remove Pierce", remove_pierce, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"+1 Pierce", add_pierce_amt, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"-1 Pierce", remove_pierce_amt, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Add Chain", add_chain, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Remove Chain", remove_chain, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"+1 Chain", add_chain_amt, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"-1 Chain", remove_chain_amt, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Add Split", add_split, rendering::gui::MenuBarTabItemType::RUN});
        world.entity().child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Remove Split", remove_split, rendering::gui::MenuBarTabItemType::RUN});
    }
}
