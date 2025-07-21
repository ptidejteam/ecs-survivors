//
// Created by Laurent Voisard on 12/22/2024.
//

#include "physics_module.h"
#include "pipeline_steps.h"

#include <raygui.h>
#include <raymath.h>
#include <modules/engine/rendering/pipeline_steps.h>

#include "components.h"
#include "queries.h"
#include "modules/engine/core/components.h"

#include "modules/engine/core/core_module.h"
#include "modules/engine/rendering/components.h"
#include "systems/add_collided_with_system.h"
#include "systems/collision_cleanup_system.h"
#include "systems/collision_detection_relationship_spatial_hashing_system.h"
#include "systems/collision_detection_spatial_hashing_system.h"
#include "systems/collision_detection_system.h"
#include "systems/collision_resolution_system.h"
#include "systems/init_spatial_hashing_grid_system.h"
#include "systems/reset_desired_velocity_system.h"
#include "systems/update_cell_entities_system.h"
#include "systems/update_grid_on_window_resized_system.h"
#include "systems/update_grid_system.h"
#include "systems/update_position_system.h"
#include "systems/update_velocity_system.h"

namespace physics {
    void PhysicsModule::register_components(flecs::world &world) {
        world.component<Velocity2D>();
        world.component<AccelerationSpeed>();
        world.component<CollidedWith>();
        world.component<ContainedIn>().add(flecs::Exclusive);
    }

    void PhysicsModule::register_queries(flecs::world &world) {
        queries::visible_collision_bodies_query = world.query_builder<core::Position2D, Collider>().with<
            rendering::Visible>().build();

        queries::box_collider_query = world.query_builder<core::Position2D, Collider>().with<BoxCollider>().build();
    }

    void PhysicsModule::register_systems(flecs::world &world) {
        m_physicsTick = world.timer().interval(PHYSICS_TICK_LENGTH);

        world.system<SpatialHashingGrid, core::GameSettings>("init grid")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind(flecs::OnStart)
                .each(systems::init_spatial_hashing_grid_system);

        world.system<SpatialHashingGrid, core::GameSettings>("update grid on window resized")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind(flecs::OnUpdate)
                .each(systems::update_grid_on_window_resized_system);

        world.observer<SpatialHashingGrid, core::GameSettings>("update grid on grid set")
                .term_at(1).singleton()
                .event(flecs::OnSet)
                .each(systems::reset_grid);

        world.system<SpatialHashingGrid, rendering::TrackingCamera, core::GameSettings, GridCell>("update grid")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .term_at(2).singleton()
                .kind(flecs::PreUpdate)
                .each(systems::update_grid_system);


        world.system<const Velocity2D, DesiredVelocity2D>("reset desired vel")
                .kind(flecs::PreUpdate)
                .multi_threaded()
                .tick_source(m_physicsTick)
                .each(systems::reset_desired_velocity_system);

        world.system<Velocity2D, const DesiredVelocity2D, const AccelerationSpeed>("Lerp Current to Desired Velocity")
                .kind<UpdateBodies>()
                .multi_threaded()
                .tick_source(m_physicsTick)
                .each(systems::update_velocity_system);

        world.system<core::Position2D, const Velocity2D>("Update Position")
                .kind<UpdateBodies>()
                .multi_threaded()
                .tick_source(m_physicsTick)
                .each(systems::update_position_system);

        world.system<SpatialHashingGrid, Collider, core::Position2D>("update entity cells")
                .term_at(0).singleton()
                .without<StaticCollider>()
                .kind<UpdateBodies>()
                .each(systems::update_cell_entities_system);


        m_collision_detection_naive_system = world.system<CollisionRecordList, const core::Position2D, const Collider>(
                    "Detect Collisions ECS (Naive Record List) non-static")
                .term_at(0).singleton()
                .with<rendering::Visible>()
                .kind<Detection>()
                .multi_threaded()
                .tick_source(m_physicsTick)
                .each(systems::collision_detection_non_static_system);
        m_collision_detection_naive_system.disable();

        // need a second pass to collide static colliders. Even when static objects are out of the screen we compute the collision
        world.system<CollisionRecordList, const core::Position2D, const Collider>(
                    "Detect Collisions ECS (Naive Record List) static")
                .term_at(0).singleton()
                .with<StaticCollider>()
                .kind<Detection>()
                .multi_threaded()
                .tick_source(m_physicsTick)
                .each(systems::collision_detection_static_system);

        m_collision_detection_spatial_hashing_system = world.system<CollisionRecordList, SpatialHashingGrid, GridCell>(
                    "Detect Collisions ECS non-static with spatial hashing")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind<Detection>()
                .multi_threaded()
                .tick_source(m_physicsTick)
                .each(systems::collision_detection_spatial_hashing_system);
        //m_collision_detection_spatial_hashing_system.disable();

        m_collision_detection_spatial_ecs = world.system<CollisionRecordList, SpatialHashingGrid, GridCell>(
                    "test collision with relationship")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind<Detection>()
                .multi_threaded()
                .tick_source(m_physicsTick)
                .each(systems::collision_detection_relationship_spatial_hashing_system);
        m_collision_detection_spatial_ecs.disable();

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
                .with<Collider>()
                .kind<CollisionCleanup>()
                .tick_source(m_physicsTick)
                .each(systems::collision_cleanup_system);

        world.system<CollisionRecordList>("Collision Cleanup List")
                .term_at(0).singleton()
                .kind<CollisionCleanup>()
                .tick_source(m_physicsTick)
                .each(systems::collision_cleanup_list_system);
    }

    void PhysicsModule::register_pipeline(flecs::world &world) {
        world.component<UpdateBodies>().add(flecs::Phase).depends_on(flecs::OnUpdate);
        world.component<Detection>().add(flecs::Phase).depends_on(flecs::OnValidate);
        world.component<Resolution>().add(flecs::Phase).depends_on(flecs::PostUpdate); // to use from external modules
        world.component<CollisionCleanup>().add(flecs::Phase).depends_on<Resolution>();
    }
}
