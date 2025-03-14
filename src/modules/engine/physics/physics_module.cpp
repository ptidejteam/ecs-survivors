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

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
        world.component<AccelerationSpeed>();
        //world.component<PhysicsTick>();
        m_physicsTick = world.timer().interval(PHYSICS_TICK_LENGTH);
    }


    void PhysicsModule::register_systems(flecs::world &world) {
        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")

                .kind(flecs::PreUpdate)
                .each([](const Velocity2D &vel, DesiredVelocity2D &desiredVel) {
                    desiredVel.value = vel.value;
                });

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .kind(flecs::PostUpdate)
                .each([&](flecs::iter &it, size_t, Velocity2D &vel, const DesiredVelocity2D &desiredVel,
                          const AccelerationSpeed &acceleration_speed) {
                    // eventually I want to use spherical linear interpolation for a smooth transition
                    vel.value = Vector2Lerp(vel.value, desiredVel.value,
                                            PHYSICS_TICK_LENGTH * acceleration_speed.value);
                });


        world.system<core::Position2D, const Velocity2D>("Update Position")
                .kind(flecs::PostUpdate)
                .each([&](const flecs::iter &it, size_t i, core::Position2D &pos, const Velocity2D &vel) {
                    pos.value = Vector2Add(pos.value, vel.value * PHYSICS_TICK_LENGTH);
                });

        auto get_all_position_and_collider_entities =
                world.query_builder<const core::Position2D, const rendering::Circle>()
                .with<rendering::Visible>()
                .build();

        s1 = world.system<const core::Position2D, const rendering::Circle>(
                    "Detect Collisions ECS (Naive add collision component)")
                .with<rendering::Visible>()
                .kind(flecs::OnValidate)
                .multi_threaded()
                .cached()
                .each([&,world, get_all_position_and_collider_entities](flecs::entity self, const core::Position2D &pos,
                                                                        const rendering::Circle &collider) {
                    flecs::entity e = world.lookup("collision_records_container");
                    get_all_position_and_collider_entities.each(
                        [&,world,e](flecs::entity other, const core::Position2D &other_pos,
                                    const rendering::Circle &other_collider) {
                            if (self == other) return;

                            if (Vector2DistanceSqr(pos.value, other_pos.value) <
                                std::pow(collider.radius + other_collider.radius, 2)) {
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

        s3 = world.system<const core::Position2D, const rendering::Circle>(
                    "Detect Collisions ECS (Naive create collision entity)")
                .with<rendering::Visible>()
                .kind(flecs::OnValidate)
                .multi_threaded()
                .cached()
                .each([&,world, get_all_position_and_collider_entities](flecs::entity self, const core::Position2D &pos,
                                                                        const rendering::Circle &collider) {
                    flecs::entity e = world.lookup("collision_records_container");
                    get_all_position_and_collider_entities.each(
                        [&,world,e](flecs::entity other, const core::Position2D &other_pos,
                                    const rendering::Circle &other_collider) {
                            if (self == other) return;

                            // if (Vector2DistanceSqr(pos.value, other_pos.value) <
                            //     std::pow(collider.radius + other_collider.radius, 2)) {
                            //     // world.entity().child_of(e).set<CollisionRecord>({self, other});
                            // }
                        });
                }).disable();

        s4 = world.system<const CollisionRecord>("Collision Resolution ECS (Naive create collision entity)")
                .kind(flecs::PostUpdate)
                .each([](const CollisionRecord &rec) {
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
                .kind(flecs::PostUpdate)
                .run([world](flecs::iter &it) {
                    // std::printf("%d\n", world.query<CollisionRecord>().count());
                    world.delete_with<CollisionRecord>();
                }).disable();

        s6 = world.system<core::Position2D>("Update Visible entities in physics")
                .with<rendering::Visible>()
                .kind(flecs::PostUpdate)
                .each([&](flecs::entity e, core::Position2D &pos) {
                    update_entity_position(e, pos);
                }).disable();

        s7 = world.observer("remove enemies from collisions when they are no longer visible")
                .with<rendering::Visible>()
                .event(flecs::OnRemove)
                .each([&](flecs::entity e) {
                    remove_entity_from_grid(e);
                }).disable();

        s8 = world.system("Detect Collisions External (Accelerated)")
                .kind(flecs::OnValidate)
                .run([&](flecs::iter &it) {
                    check_collisions();
                }).disable();

        s9 = world.system("Resolve Collisions External (Accelerated)")
                .kind(flecs::PostUpdate)
                .run([&, world](flecs::iter &it) {
                    resolve_collisions(world);
                }).disable();
    }
}
