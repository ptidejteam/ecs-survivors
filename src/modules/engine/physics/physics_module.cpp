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
        //world.component<CircleCollider>().is_a<Collider>();
        //world.component<BoxCollider>().is_a<Collider>();
    }

    void PhysicsModule::register_queries(flecs::world &world) {
        queries::visible_collision_bodies_query = world.query_builder<core::Position2D, Collider>().with<
            rendering::Visible>().build();

        queries::box_collider_query = world.query_builder<core::Position2D, Collider>().with<BoxCollider>().build();
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
                .kind<Detection>().with<rendering::Visible>()
                .tick_source(m_physicsTick)
                .multi_threaded()
                .each(systems::collision_detection_all_system);

        world.system<CollisionRecordList>("Collision Resolution ECS (Naive Record List)")
                .term_at(0).singleton()
                .kind<Resolution>()
                .tick_source(m_physicsTick)
                .each(systems::collision_resolution_system);

        world.system<CollisionRecordList>("Add CollidedWith Component")
                .term_at(0).singleton()
                .kind<Resolution>()
                .tick_source(m_physicsTick)
                .each(systems::add_collided_with_system);

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
