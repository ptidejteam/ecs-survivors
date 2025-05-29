//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"
#include "pipeline_steps.h"

#include <raygui.h>
#include <raymath.h>

#include "components.h"
#include "modules/engine/core/components.h"

#include "modules/engine/core/core_module.h"
#include "modules/engine/rendering/components.h"
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


    void PhysicsModule::register_systems(flecs::world &world) {
        m_physicsTick = world.timer().interval(PHYSICS_TICK_LENGTH);

        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")
                .kind(flecs::PreUpdate)
                .tick_source(m_physicsTick)
                .each(reset_desired_velocity_system);

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .kind(flecs::OnValidate)
                .tick_source(m_physicsTick)
                .write<Velocity2D>()
                .each(update_velocity_system);

        world.system<core::Position2D, const Velocity2D>("Update Position")
                .kind(flecs::OnValidate)
                .tick_source(m_physicsTick)
                .write<core::Position2D>()
                .each(update_position_system);

        auto get_all_collision_bodies =
                world.query_builder<const core::Position2D, const Collider>()
                .with<rendering::Visible>()
                .build();

        world.system<CollisionRecordList>(
                    "Detect Collisions ECS (Naive Record List)")
                .term_at(0).singleton()
                .kind(flecs::OnValidate)
                .tick_source(m_physicsTick)
                .immediate()
                .each([get_all_collision_bodies](CollisionRecordList &list) {
                    collision_detection_system(get_all_collision_bodies, list);
                });

        world.system<CollisionRecordList>("Collision Resolution ECS (Naive Record List)")
                .kind(flecs::PostUpdate)
                .tick_source(m_physicsTick)
                .each(collision_resolution_system);

        world.system("Collision Cleanup")
                .kind(flecs::OnStore)
                .tick_source(m_physicsTick)
                .with<CollidedWith>(flecs::Wildcard)
                .immediate()
                .each(collision_cleanup_system);
    }

    void PhysicsModule::register_pipeline(flecs::world &world) {
        world.component<Detection>().add(flecs::Phase).depends_on(flecs::OnValidate);
        world.component<CollisionDetected>().add(flecs::Phase).depends_on(flecs::PostUpdate);
        world.component<Resolution>().add(flecs::Phase).depends_on<CollisionDetected>();
        world.component<CollisionCleanup>().add(flecs::Phase).depends_on(flecs::OnStore);
    }
}
