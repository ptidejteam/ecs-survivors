//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"

#include <chrono>
#include <raygui.h>

#include "components.h"
#include "modules/engine/core/components.h"
#include <raymath.h>
#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

#include "game.h"
#include "modules/engine/core/core_module.h"
#include "modules/engine/rendering/components.h"
#include "modules/engine/rendering/gui/components.h"

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
        world.component<AccelerationSpeed>();
        world.component<CollidedWith>().add(flecs::PairIsTag);
        world.component<ContainedBy>();
        world.component<Cell>();
        m_physicsTick = world.timer().interval(PHYSICS_TICK_LENGTH);
    }


    void PhysicsModule::register_systems(flecs::world &world) {
        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")
                .kind(flecs::PreUpdate)
                .each([](const Velocity2D &vel, DesiredVelocity2D &desiredVel) {
                    desiredVel.value = vel.value;
                });

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .kind(flecs::OnValidate)
                .multi_threaded()
                .write<Velocity2D>()
                .each([&](flecs::iter &it, size_t, Velocity2D &vel, const DesiredVelocity2D &desiredVel,
                          const AccelerationSpeed &acceleration_speed) {
                    // eventually I want to use spherical linear interpolation for a smooth transition
                    vel.value = Vector2Lerp(vel.value, desiredVel.value,
                                            PHYSICS_TICK_LENGTH * acceleration_speed.value);
                });

        world.system<core::Position2D, const Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>(
                    "Update Position")
                .kind(flecs::OnValidate)
                .write<core::Position2D>()
                .each([&](const flecs::iter &it, size_t i, core::Position2D &pos, const Velocity2D &vel,
                          const DesiredVelocity2D &desiredVel,
                          const AccelerationSpeed &acceleration_speed) {
                    auto vnplus1 = Vector2Lerp(vel.value, desiredVel.value,
                                               PHYSICS_TICK_LENGTH * acceleration_speed.value);
                    pos.value = Vector2Add(pos.value, vnplus1 * PHYSICS_TICK_LENGTH);
                });

        auto get_all_position_and_collider_entities =
                world.query_builder<const core::Position2D, const rendering::Circle>()
                .with<rendering::Visible>()
                .build();

        s1 = world.system<const core::Position2D, const rendering::Circle>(
                    "Detect Collisions ECS (Naive add collision component)")
                .with<rendering::Visible>()
                .kind(flecs::OnValidate)
                .each([get_all_position_and_collider_entities](flecs::entity self, const core::Position2D &pos,
                                                               const rendering::Circle &collider) {
                    get_all_position_and_collider_entities.each(
                        [&](flecs::entity other, const core::Position2D &other_pos,
                            const rendering::Circle &other_collider) {
                            if (self.id() >= other.id()) return;

                            float rad = collider.radius + other_collider.radius;
                            if (Vector2DistanceSqr(pos.value, other_pos.value) < rad * rad) {
                                self.add<CollidedWith>(other);
                            } else {
                                self.remove<CollidedWith>(other);
                            }
                        });
                }).disable();


        s2 = world.system("Collision Resolution ECS (Naive add collision component)")
                .with<CollidedWith>(flecs::Wildcard)
                .kind(flecs::PostUpdate)
                .each([](flecs::iter &it, size_t i) {
                    flecs::entity other = it.pair(0).second(); // Colliding entity
                    flecs::entity self = it.entity(i); // Current entity

                    Vector2 mypos = self.get<core::Position2D>()->value;
                    Vector2 otherPos = other.get<core::Position2D>()->value;
                    float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
                                           radius;

                    // Find the distance and adjust to resolve the overlap
                    Vector2 direction = otherPos - mypos;
                    Vector2 moveDirection = Vector2Normalize(direction);
                    float overlap = combinedRadius - Vector2Length(direction);

                    // Move the entities apart by the amount of overlap
                    Vector2 move = moveDirection * overlap * 0.5f;

                    self.set<Velocity2D>({self.get<Velocity2D>()->value - move * 2.f});
                    other.set<Velocity2D>({other.get<Velocity2D>()->value + move * 2.f});

                    // Resolve by adjusting positions
                    self.set<core::Position2D>({mypos - move / 2.f}); // Move the current entity
                    other.set<core::Position2D>({otherPos + move / 2.f}); // Move the other entity
                }).disable();

        flecs::entity collision_records_container = world.lookup("collision_records_container");
        s3 = world.system(
                    "Detect Collisions ECS (Naive create collision entity)")
                .with<rendering::Visible>()
                .kind(flecs::OnValidate)
                .immediate()
                .run([world,collision_records_container, get_all_position_and_collider_entities](flecs::iter &it) {
                    ZoneScopedN("Collision Detection");
                    get_all_position_and_collider_entities.each([&](flecs::entity self, const core::Position2D &pos,
                                                                    const rendering::Circle &collider) {
                        get_all_position_and_collider_entities.each(
                            [&](flecs::entity other, const core::Position2D &other_pos,
                                const rendering::Circle &other_collider) {
                                ZoneScopedN("Collide");
                                if (self.id() >= other.id()) return;
                                {
                                    float rad = collider.radius + other_collider.radius;
                                    if (Vector2DistanceSqr(pos.value, other_pos.value) < rad * rad) {
                                        world.entity().child_of(collision_records_container).set<CollisionRecord>({
                                            self, other
                                        });
                                    }
                                }
                            });
                    });
                }).disable();

        // auto recs_query =
        s4 = world.system<const CollisionRecord>("Collision Resolution ECS (Naive create collision entity)")
                .kind(flecs::PostUpdate)
                .each([](const CollisionRecord &rec) {
                    ZoneScopedN("Collision Resolution");
                    flecs::entity other = rec.b; // Colliding entity
                    flecs::entity self = rec.a; // Current entity
                    Vector2 mypos = self.get<core::Position2D>()->value;
                    Vector2 otherPos = other.get<core::Position2D>()->value;
                    float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
                                           radius;

                    // Find the distance and adjust to resolve the overlap
                    Vector2 direction = otherPos - mypos;
                    Vector2 moveDirection = Vector2Normalize(direction);
                    float overlap = combinedRadius - Vector2Length(direction);

                    // Move the entities apart by the amount of overlap
                    Vector2 move = moveDirection * overlap * 0.5f;

                    self.set<Velocity2D>({self.get<Velocity2D>()->value - move});
                    other.set<Velocity2D>({other.get<Velocity2D>()->value + move});

                    // Resolve by adjusting positions
                    self.set<core::Position2D>({mypos - move / 2.f}); // Move the current entity
                    other.set<core::Position2D>({otherPos + move / 2.f}); // Move the other entity
                }).disable();

        s5 = world.system("Collision Cleanup ECS (Naive create collision entity)")
                .kind(flecs::OnStore)
                .run([world](flecs::iter &it) {
                    world.delete_with<CollisionRecord>();
                }).disable();

        auto get_visible_entity_positions = world.query_builder<const core::Position2D>()
                .with<rendering::Visible>()
                .build();

        s6 = world.system("Update Visible entities in physics")
                .with<core::Position2D>()
                .with<rendering::Visible>()
                .kind(flecs::OnUpdate)
                .each([&](flecs::entity e) {
                    update_entity_position(e);
                }).disable();


        remove_entity_to_physics = world.observer("remove enemies from collisions when they are no longer visible")
                .with<rendering::Visible>()
                .event(flecs::OnRemove)
                .each([&](flecs::entity e) {
                    remove_entity_from_grid(e);
                }).disable();

        add_entity_to_physics = world.observer("add enemies from collisions when they become visible")
                .with<rendering::Visible>()
                .event(flecs::OnAdd)
                .each([&](flecs::entity e) {
                    add_entity_to_grid(e);
                }).disable();

        s8 = world.system<const Cell>("Detect Collisions External (Accelerated)")
                .kind(flecs::OnValidate)
                .multi_threaded()
                .each([&, world](const Cell &cell) {
                    ZoneScoped;
                    detect_collisions(cell);
                }).disable();

        s9 = world.system("Resolve Collisions External (Accelerated)")
                .kind(flecs::PostUpdate)
                .run([&](flecs::iter &it) {
                    resolve_collisions();
                }).disable();

        world.observer<EventBus>("Create partition cells")
                .event<core::WindowResizedEvent>()
                .each([&, world](EventBus &bus) {
                    std::printf("hello\n");
                    core::GameSettings settings = *world.get<core::GameSettings>();
                    world.delete_with<Cell>();
                    for (int x = 0; x < ceil(settings.windowWidth / m_spatial_hash->
                                             get_cell_size()) + 1; x++) {
                        for (int y = 0; y < ceil(settings.windowHeight / m_spatial_hash->get_cell_size()) + 1; y++) {
                            world.entity().set<Cell>({
                                x, y
                            });
                        }
                    }
                });


        // world.system<const core::Position2D>("update entity parent cells")
        //         .kind(flecs::OnValidate)
        //         .each([&,world, cell_query](flecs::entity e, const core::Position2D &pos) {
        //             cell_query.each([&,e,pos](flecs::entity parent, Cell &cell) {
        //                 std::pair<int, int> h = hash(pos.value);
        //                 if (h.first == cell.x && h.second == cell.y) {
        //                     e.add<ContainedBy>(parent);
        //                 } else {
        //                     e.remove<ContainedBy>(parent);
        //                 }
        //             });
        //         });

        // world.system<const core::Position2D, const rendering::Circle>("Collision Detection ECS Spatial Hashing")
        //         .with<ContainedBy>(flecs::Wildcard)
        //         .immediate()
        //         .kind(flecs::OnValidate)
        //         .each([&,world](flecs::iter &it, size_t i, const core::Position2D &self_pos,
        //                         const rendering::Circle &self_col) {
        //             flecs::entity cell = it.pair(2).second(); // Colliding entity
        //
        //             flecs::entity self = it.entity(i);
        //             flecs::entity e = world.lookup("collision_records_container");
        //             m_cell_entities_queries[cell]
        //                     .each([&, world,e, self, self_pos, self_col](flecs::entity other,
        //                                                               const core::Position2D &other_pos,
        //                                                               const rendering::Circle &other_col) {
        //                         if (self == other) return;
        //                         // std::printf("comparing two entities\n");
        //                         if (Vector2DistanceSqr(self_pos.value, other_pos.value) <
        //                             std::pow(self_col.radius + other_col.radius, 2)) {
        //                             world.entity().set<CollisionRecord>({self, other});
        //                             //std::printf("%d\n", world.query<CollisionRecord>().count());
        //                             //std::printf("collision detected\n");
        //                         }
        //                     });
        //         });
        //
        // s4 = world.system<const CollisionRecord>("Collision Resolution ECS")
        //         .kind(flecs::PostUpdate)
        //         .each([](const CollisionRecord &rec) {
        //             flecs::entity other = rec.b; // Colliding entity
        //             flecs::entity self = rec.a; // Current entity
        //
        //             Vector2 mypos = self.get<core::Position2D>()->value;
        //             Vector2 otherPos = other.get<core::Position2D>()->value;
        //             float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
        //                                    radius;
        //
        //             // Find the distance and adjust to resolve the overlap
        //             Vector2 direction = otherPos - mypos;
        //             Vector2 moveDirection = Vector2Normalize(direction);
        //             float overlap = combinedRadius - Vector2Length(direction);
        //
        //             // Move the entities apart by the amount of overlap
        //             Vector2 move = moveDirection * overlap * 0.5f;
        //
        //             self.set<Velocity2D>({self.get<Velocity2D>()->value - move});
        //             other.set<Velocity2D>({other.get<Velocity2D>()->value + move});
        //
        //             // Resolve by adjusting positions
        //             self.set<core::Position2D>({mypos - move / 2.f}); // Move the current entity
        //             other.set<core::Position2D>({otherPos + move / 2.f}); // Move the other entity
        //         }).disable();
        //
        // s5 = world.system("Collision Cleanup")
        //         .kind(flecs::OnStore)
        //         .run([world](flecs::iter &it) {
        //             // std::printf("%d\n", world.query<CollisionRecord>().count());
        //             world.delete_with<CollisionRecord>();
        //         }).disable();
    }
}
