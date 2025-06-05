//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"
#include "pipeline_steps.h"

#include <raygui.h>
#include <raymath.h>

#include "components.h"
#include "queries.h"
#include "modules/engine/core/components.h"

#include "modules/engine/core/core_module.h"
#include "modules/engine/rendering/components.h"
#include "systems/add_collided_with_system.h"
#include "systems/collision_cleanup_system.h"
#include "systems/collision_detection_system.h"
#include "systems/collision_resolution_system.h"
#include "systems/reset_desired_velocity_system.h"
#include "systems/update_position_system.h"
#include "systems/update_velocity_system.h"

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
        world.component<AccelerationSpeed>();
        world.component<CollidedWith>();
    }

    void PhysicsModule::register_queries(flecs::world &world) {
        queries::visible_collision_bodies_query = world.query_builder<core::Position2D, Collider>().with<
            rendering::Visible>().build();

        queries::box_collider_query = world.query_builder<BoxCollider>().build();
    }


    void PhysicsModule::register_systems(flecs::world &world) {
        m_physicsTick = world.timer().interval(PHYSICS_TICK_LENGTH);

        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")
                .kind(flecs::PreUpdate)
                .tick_source(m_physicsTick)
                .multi_threaded()
                .each(systems::reset_desired_velocity_system);

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .kind<UpdateBodies>()
                .tick_source(m_physicsTick)
                .multi_threaded()
                .each(systems::update_velocity_system);

        world.system<core::Position2D, const Velocity2D>("Update Position")
                .kind<UpdateBodies>()
                .tick_source(m_physicsTick)
                .multi_threaded()
                .each(systems::update_position_system);

        world.system<CollisionRecordList, const core::Position2D, const Collider>(
                    "Detect Collisions ECS (Naive Record List)")
                .term_at(0).singleton()
                .kind<Detection>()
                .tick_source(m_physicsTick)
                .multi_threaded()
                .each(systems::collision_detection_system);

        world.system<CollisionRecordList, const BoxCollider>(
                    "Detect Collisions Environment ECS (Naive Record List)")
                .term_at(0).singleton()
                .kind<Detection>()
                .tick_source(m_physicsTick)
                .multi_threaded()
                .each([](flecs::iter &self_it, size_t self_id, CollisionRecordList &list,
                         const BoxCollider &collider) {
                    flecs::world stage_world = self_it.world();

                    // Build a staged query, and filter
                    auto visible_query = stage_world.query_builder<const core::Position2D, const Collider>()
                            .with<rendering::Visible>().build();

                    visible_query.each(
                        [&](flecs::iter &other_it, size_t other_id, const core::Position2D &other_pos,
                            const Collider &other_collider) {
                            //if (self_id > other_id) return;

                            float recCenterX = collider.rec.x + collider.rec.width / 2.0f;
                            float recCenterY = collider.rec.y + collider.rec.height / 2.0f;

                            float halfWidth = collider.rec.width / 2.0f;
                            float halfHeight = collider.rec.height / 2.0f;

                            float dx = other_pos.value.x - recCenterX;
                            float dy = other_pos.value.y - recCenterY;

                            float absDx = fabsf(dx);
                            float absDy = fabsf(dy);

                            if (absDx > (halfWidth + other_collider.radius)) return;
                            if (absDy > (halfHeight + other_collider.radius)) return;


                            flecs::entity other = other_it.entity(other_id);
                            if (absDx <= halfWidth || absDy <= halfHeight) {
                                //std::cout << "collided 1" << std::endl;
                                // Side collision — resolve with axis-aligned MTV
                                float overlapX = (halfWidth + other_collider.radius) - absDx;
                                float overlapY = (halfHeight + other_collider.radius) - absDy;

                                if (overlapX < overlapY) {
                                    other.set<core::Position2D>({
                                        other_pos.value.x + ((dx < 0) ? -overlapX : overlapX), other_pos.value.y
                                    });
                                } else {
                                    other.set<core::Position2D>({
                                        other_pos.value.x, other_pos.value.y + ((dy < 0) ? -overlapY : overlapY)
                                    });
                                }

                                if (!other_collider.correct_position) {
                                    other.add<core::DestroyAfterFrame>();
                                }

                                return;
                            }

                            // Corner collision
                            float cornerDx = absDx - halfWidth;
                            float cornerDy = absDy - halfHeight;

                            float cornerDistSq = cornerDx * cornerDx + cornerDy * cornerDy;
                            float radius = other_collider.radius;

                            if (cornerDistSq < radius * radius) {
                                //std::cout << "collided 2" << std::endl;
                                float dist = sqrtf(cornerDistSq);

                                if (dist == 0.0f) dist = 0.01f; // Avoid divide by zero

                                float overlap = radius - dist;
                                float nx = cornerDx / dist;
                                float ny = cornerDy / dist;

                                other.set<core::Position2D>({
                                    other_pos.value.x + nx * overlap * ((dx < 0) ? -1.0f : 1.0f),
                                    other_pos.value.y + ny * overlap * ((dy < 0) ? -1.0f : 1.0f)
                                });
                                if (!other_collider.correct_position) {
                                    other.add<core::DestroyAfterFrame>();
                                }
                            }
                        });
                });

        world.system<CollisionRecordList>("Add CollidedWith Component")
                .term_at(0).singleton()
                .kind<Resolution>()
                .tick_source(m_physicsTick)
                .each(systems::add_collided_with_system);

        world.system<CollisionRecordList>("Collision Resolution ECS (Naive Record List)")
                .term_at(0).singleton()
                .kind<Resolution>()
                .tick_source(m_physicsTick)
                .each(systems::collision_resolution_system);

        world.system("Collision Cleanup")
                .kind<CollisionCleanup>()
                .tick_source(m_physicsTick)
                .with<CollidedWith>(flecs::Wildcard)
                .multi_threaded()
                .each(systems::collision_cleanup_system);
    }

    void PhysicsModule::register_pipeline(flecs::world &world) {
        world.component<UpdateBodies>().add(flecs::Phase).depends_on(flecs::OnUpdate);
        world.component<Detection>().add(flecs::Phase).depends_on(flecs::OnValidate);
        world.component<Resolution>().add(flecs::Phase).depends_on(flecs::PostUpdate); // to use from external modules
        world.component<CollisionCleanup>().add(flecs::Phase).depends_on<Resolution>();
    }
}
