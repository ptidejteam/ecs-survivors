//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"

#include <raygui.h>
#include <raymath.h>

#include "components.h"
#include "modules/engine/core/components.h"

#include "modules/engine/core/core_module.h"
#include "modules/engine/rendering/components.h"

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
        world.component<AccelerationSpeed>();
        world.component<CollidedWith>();
        m_physicsTick = world.timer().interval(PHYSICS_TICK_LENGTH);
    }


    void PhysicsModule::register_systems(flecs::world &world) {
        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")
                .kind(flecs::PreUpdate)
                .tick_source(m_physicsTick)
                .each([](const Velocity2D &vel, DesiredVelocity2D &desiredVel) {
                    desiredVel.value = vel.value;
                });

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .kind(flecs::OnValidate)
                .tick_source(m_physicsTick)
                .write<Velocity2D>()
                .each([&](flecs::iter &it, size_t, Velocity2D &vel, const DesiredVelocity2D &desiredVel,
                          const AccelerationSpeed &acceleration_speed) {
                    // eventually I want to use spherical linear interpolation for a smooth transition
                    vel.value = Vector2Lerp(vel.value, desiredVel.value,
                                            std::min(PHYSICS_TICK_LENGTH, it.delta_system_time()) * acceleration_speed.
                                            value);
                });

        world.system<core::Position2D, const Velocity2D>(
                    "Update Position")
                .kind(flecs::OnValidate)
                .tick_source(m_physicsTick)
                .write<core::Position2D>()
                .each([&](const flecs::iter &it, size_t i, core::Position2D &pos, const Velocity2D &vel) {
                    pos.value = Vector2Add(
                        pos.value, vel.value * std::min(PHYSICS_TICK_LENGTH, it.delta_system_time()));
                });

        auto get_all_position_and_collider_entities =
                world.query_builder<const core::Position2D, const Collider>()
                .with<rendering::Visible>()
                .build();

        world.system<CollisionRecordList>(
                    "Detect Collisions ECS (Naive Record List)")
                .term_at(0).singleton()
                .kind(flecs::OnValidate)
                .tick_source(m_physicsTick)
                .immediate()
                .each([&, get_all_position_and_collider_entities](CollisionRecordList &list) {
                    get_all_position_and_collider_entities.each(
                        [&](flecs::iter &self_it, size_t self_id,
                            const core::Position2D &pos,
                            const Collider &collider) {
                            get_all_position_and_collider_entities.each(
                                [&](flecs::iter &other_it, size_t other_id, const core::Position2D &other_pos,
                                    const Collider &other_collider) {
                                    if (self_id > other_id) return;
                                    if ((collider.collision_filter & other_collider.collision_type) == none) return;

                                    float rad = collider.radius + other_collider.radius;
                                    if (Vector2DistanceSqr(pos.value, other_pos.value) < rad * rad) {
                                        flecs::entity self = self_it.entity(self_id);
                                        flecs::entity other = other_it.entity(other_id);

                                        if(collider.correct_position && other_collider.correct_position)
                                            list.records.push_back({self, other});

                                        if ((collider.collision_type & other_collider.collision_type) == none) {
                                            self.add<CollidedWith>(other);
                                            other.add<CollidedWith>(self);
                                        }
                                    }
                                });
                        });
                });

        world.system<CollisionRecordList>("Collision Resolution ECS (Naive Record List)")
                .kind(flecs::PostUpdate)
                .tick_source(m_physicsTick)
                .each([](CollisionRecordList &rec) {
                    for (auto &record: rec.records) {
                        flecs::entity other = record.b; // Colliding entity
                        flecs::entity self = record.a; // Current entity
                        Vector2 mypos = self.get<core::Position2D>()->value;
                        Vector2 otherPos = other.get<core::Position2D>()->value;
                        float combinedRadius = self.get<Collider>()->radius +
                                               other.get<Collider>()->radius;

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
                    }
                });

        world.system<CollisionRecordList>("Collision Cleanup ECS (Naive Record List)")
                .term_at(0).singleton()
                .kind(flecs::OnStore)
                .tick_source(m_physicsTick)
                .each([](CollisionRecordList &holder) {
                    holder.records.clear();
                });

        world.system("Collision Cleanup ECS 2")
                .kind(flecs::OnStore)
                .tick_source(m_physicsTick)
                .with<CollidedWith>(flecs::Wildcard)
                .immediate()
                .each([](flecs::iter &it, size_t i) {
                    flecs::entity self = it.entity(i);
                    flecs::entity other = it.pair(0).second();
                    self.remove<CollidedWith>(other);
                });
    }
}
