//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"

#include <chrono>
#include <raygui.h>

#include "components.h"
#include "modules/engine/core/components.h"
#include <raymath.h>

#include "modules/engine/rendering/components.h"

using namespace core;

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
        world.component<AccelerationSpeed>();
        //world.component<PhysicsTick>();
        m_physicsTick = world.timer().interval(PHYSICS_TICK_LENGTH);
    }


    void PhysicsModule::register_systems(flecs::world &world) {
        // world.system<Position2D, rendering::Circle>("On start set the entities cells")
        //         .kind(flecs::OnStart)
        //         .each([](flecs::entity e, Position2D &pos, rendering::Circle &circle) {
        //             e.set<Cell>({std::pair{-1, -1}});
        //         });

        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")
                .tick_source(m_physicsTick)
                .kind(flecs::PreUpdate)
                .each([](const Velocity2D &vel, DesiredVelocity2D &desiredVel) {
                    desiredVel.value = vel.value;
                });

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .tick_source(m_physicsTick)
                .kind(flecs::PostUpdate)
                .each([&](flecs::iter &it, size_t, Velocity2D &vel, const DesiredVelocity2D &desiredVel,
                          const AccelerationSpeed &acceleration_speed) {
                    // eventually I want to use spherical linear interpolation for a smooth transition
                    vel.value = Vector2Lerp(vel.value, desiredVel.value,
                                            PHYSICS_TICK_LENGTH * acceleration_speed.value);
                });

        world.observer<const rendering::Visible>("remove enemies from collisions when they are no longer visible")
                .event(flecs::OnRemove)
                .each([&](flecs::entity e, const rendering::Visible &visible) {
                    remove_entity_from_grid(e);
                });

        // world.observer()
        //     .with<rendering::Visible>()
        //     .event(flecs::OnAdd)
        //     .each([&](flecs::entity e) {
        //        add_entity_to_grid(e);
        //     });

        world.system<Position2D, const Velocity2D>("Update Position")
                .tick_source(m_physicsTick)
                .kind(flecs::PostUpdate)
                .each([&](const flecs::iter &it, size_t i, Position2D &pos, const Velocity2D &vel) {
                    pos.value = Vector2Add(pos.value, vel.value * PHYSICS_TICK_LENGTH);
                });

        world.system<Position2D>("Update Visible entities in physics")
                .with<rendering::Visible>()
                .tick_source(m_physicsTick)
                .kind(flecs::PostUpdate)
                .each([&](const flecs::iter &it, size_t i, Position2D &pos) {
                        update_entity_position(it.entity(i), pos);
                });

        auto get_all_position_and_collider_entities =
            world.query_builder<const Position2D, const rendering::Circle>()
                .with<rendering::Visible>()
                .build();


        world.system<const Position2D, const rendering::Circle>("Detect Collisions (Naive)")
                .with<rendering::Visible>()
                .tick_source(m_physicsTick)
                .kind(flecs::PostUpdate)
                .each([&, get_all_position_and_collider_entities](flecs::entity self, const Position2D &pos,
                                                                  const rendering::Circle &collider) {
                    get_all_position_and_collider_entities.each(
                        [&](flecs::entity other, const Position2D &other_pos, const rendering::Circle &other_collider) {
                            if (self == other) return;
                            if (self.has<CollidedWith>(other)) return;

                            if (Vector2DistanceSqr(pos.value, other_pos.value) <
                                std::pow(collider.radius + other_collider.radius, 2)) {
                                self.add<CollidedWith>(other);
                                other.add<CollidedWith>(self);
                            }
                        });
                }).disable();



        world.system("Collision Resolution ECS (Naive)")
                .with<CollidedWith>(flecs::Wildcard)
                .tick_source(m_physicsTick)
                .kind(flecs::OnValidate)
                .each([](flecs::iter &it, size_t i) {
                    flecs::entity other = it.pair(0).second(); // Colliding entity
                    flecs::entity self = it.entity(i); // Current entity

                    Vector2 mypos = self.get<Position2D>()->value;
                    Vector2 otherPos = other.get<Position2D>()->value;
                    float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->radius;

                    // Find the distance and adjust to resolve the overlap
                    Vector2 direction = otherPos - mypos;
                    float distanceSqr = Vector2LengthSqr(direction);

                    if (distanceSqr < std::pow(combinedRadius, 2)) {
                        Vector2 moveDirection = Vector2Normalize(direction);
                        float overlap = combinedRadius - Vector2Length(direction);

                        // Move the entities apart by the amount of overlap
                        Vector2 move = moveDirection * overlap * 0.5f;

                        self.set<Velocity2D>({self.get<Velocity2D>()->value - move * 2.f});
                        other.set<Velocity2D>({other.get<Velocity2D>()->value + move * 2.f});

                        // Resolve by adjusting positions
                        self.set<core::Position2D>({mypos - move / 2.f}); // Move the current entity
                        other.set<core::Position2D>({otherPos + move / 2.f}); // Move the other entity
                    }
                }).disable();

        world.system("Collision Cleanup")
                .kind(flecs::OnLoad)
                .with<CollidedWith>(flecs::Wildcard)
                .each([](flecs::iter &it, size_t i) {
                    flecs::entity other = it.pair(0).second(); // Colliding entity
                    flecs::entity e = it.entity(i); // Current entity
                    e.remove<CollidedWith>(other);
                }).disable();

        world.system("Detect Collisions External (Accelerated)")
                .tick_source(m_physicsTick)
                .kind(flecs::PostUpdate)
                .run([&](flecs::iter &it) {
                    check_collisions();
                });

        world.system("Resolve Collisions External (Accelerated)")
                .tick_source(m_physicsTick)
                .kind(flecs::OnValidate)
                .run([&](flecs::iter &it) {
                    resolve_collisions();
                });
    }
}
