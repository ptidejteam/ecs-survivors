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
        world.component<ContainedIn>().add(flecs::Exclusive).add(flecs::DontFragment);
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

        world.system<SpatialHashingGrid, core::GameSettings>("init grid")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind(flecs::OnStart)
                .each([](flecs::iter &it, size_t i, SpatialHashingGrid &hashing_grid, core::GameSettings &settings) {
                    m_cell_query.clear();
                    for (int y = -1; y < std::ceil((float) settings.window_height / (float) hashing_grid.cell_size) + 1;
                         y++) {
                        for (int x = -1; x < std::ceil(settings.window_width / hashing_grid.cell_size) + 1; x++) {
                            flecs::entity e = it.world().entity().set<GridCell>({x, y});
                            hashing_grid.cells[std::make_pair(x, y)] = e;
                        }
                    }
                });

        world.system<SpatialHashingGrid, core::GameSettings>("update grid on window resized")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind(flecs::OnUpdate)
                .each([](flecs::iter &it, size_t i, SpatialHashingGrid &hashing_grid, core::GameSettings &settings) {
                    if (IsWindowResized()) {
                        for (auto cell: hashing_grid.cells) {
                            cell.second.destruct();
                        }
                        hashing_grid.cells.clear();
                        for (int y = -1; y < std::ceil((float) settings.window_height / (float) hashing_grid.cell_size)+ 1; y++) {
                            for (int x = -1; x < std::ceil(settings.window_width / hashing_grid.cell_size) + 1; x++) {
                                flecs::entity e = it.world().entity().set<GridCell>({x, y});
                                hashing_grid.cells[std::make_pair(x, y)] = e;
                            }
                        }
                    }
                });

        world.observer<SpatialHashingGrid, core::GameSettings>("update grid on grid set")
                .term_at(1).singleton()
                .event(flecs::OnSet)
                .each([](flecs::iter &it, size_t i, SpatialHashingGrid &hashing_grid, core::GameSettings &settings) {
                    for (auto cell: hashing_grid.cells) {
                        cell.second.destruct();
                    }
                    hashing_grid.cells.clear();
                    m_cell_query.clear();
                    for (int y = -1; y < std::ceil((float) settings.window_height / (float) hashing_grid.cell_size)+ 1; y++) {
                            for (int x = -1; x < std::ceil(settings.window_width / hashing_grid.cell_size) + 1; x++) {
                                flecs::entity e = it.world().entity().set<GridCell>({x, y});
                                hashing_grid.cells[std::make_pair(x, y)] = e;
                            }
                        }
                    std::cout << "Updating Grid" << std::endl;
                });

        world.system<SpatialHashingGrid, rendering::TrackingCamera, core::GameSettings, GridCell>("update grid")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .term_at(2).singleton()
                .kind(flecs::PreUpdate)
                .each([](flecs::iter &it, size_t i, SpatialHashingGrid &grid, rendering::TrackingCamera &cam,
                         core::GameSettings &settings, GridCell &cell) {
                    grid.offset = cam.camera.target - Vector2{
                                      settings.window_width / 2.0f, settings.window_height / 2.0f
                                  };

                    cell.entities.clear();
                });


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
                .each([](flecs::entity e, SpatialHashingGrid &grid, Collider &col, core::Position2D &pos) {
                    int cell_pos_x = std::floor((pos.value.x - grid.offset.x) / grid.cell_size);
                    int cell_pos_y = std::floor((pos.value.y - grid.offset.y) / grid.cell_size);

                    if (!grid.cells.contains(std::make_pair(cell_pos_x, cell_pos_y))) {
                        return;
                    }

                    flecs::entity cell = grid.cells[std::make_pair(cell_pos_x, cell_pos_y)];
                    cell.get_mut<GridCell>().entities.push_back(e);
                    //e.add<ContainedIn>(cell);
                });


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
                .each([](flecs::entity e, CollisionRecordList &list, SpatialHashingGrid &grid, GridCell &cell) {
                    std::vector<CollisionRecord> collisions;
                    for (int offset_y = -1; offset_y <= 1; offset_y++) {
                        for (int offset_x = -1; offset_x <= 1; offset_x++) {
                            int x = cell.x + offset_x;
                            int y = cell.y + offset_y;
                            if (!grid.cells.contains(std::make_pair(x, y))) continue;

                            const GridCell neighbour = grid.cells[std::make_pair(x, y)].get<GridCell>();

                            for (int i = 0; i < cell.entities.size(); i++) {
                                flecs::entity self = cell.entities[i];
                                if (!self.is_alive()) continue;
                                const core::Position2D pos = cell.entities[i].get<core::Position2D>();
                                const Collider collider = cell.entities[i].get<Collider>();
                                for (int j = 0; j < neighbour.entities.size(); j++) {
                                    flecs::entity other = neighbour.entities[j];
                                    if (!other.is_alive()) continue;
                                    const core::Position2D other_pos = neighbour.entities[j].get<core::Position2D>();
                                    const Collider other_collider = neighbour.entities[j].get<Collider>();
                                    if (self.id() <= other.id()) continue;

                                    if ((collider.collision_filter & other_collider.collision_type) == none) continue;

                                    Rectangle self_rec = {
                                        pos.value.x + collider.bounds.x, pos.value.y + collider.bounds.y,
                                        collider.bounds.width,
                                        collider.bounds.height
                                    };
                                    Rectangle other_rec = {
                                        other_pos.value.x + other_collider.bounds.x,
                                        other_pos.value.y + other_collider.bounds.y,
                                        other_collider.bounds.width, other_collider.bounds.height
                                    };

                                    if (CheckCollisionRecs(self_rec, other_rec)) {
                                        //std::cout << "Collision detected" << std::endl;
                                        collisions.push_back({self, other});
                                    }
                                }
                            }
                        }
                    }
                    if (collisions.empty()) return;
                    systems::list_mutex.lock();
                    list.records.insert(list.records.end(), collisions.begin(), collisions.end());
                    systems::list_mutex.unlock();
                });
        m_collision_detection_spatial_hashing_system.disable();

        m_collision_detection_spatial_ecs = world.system<CollisionRecordList, SpatialHashingGrid, GridCell>(
                    "test collision with relationship")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind<Detection>()
                //.multi_threaded()
                .tick_source(m_physicsTick)
                .each([](flecs::iter &it, size_t i, CollisionRecordList &list, SpatialHashingGrid &grid,
                         GridCell &cell) {
                    flecs::entity current_cell = it.entity(i);

                    std::vector<CollisionRecord> collisions;
                    for (int offset_y = -1; offset_y <= 1; offset_y++) {
                        for (int offset_x = -1; offset_x <= 1; offset_x++) {
                            int x = cell.x + offset_x;
                            int y = cell.y + offset_y;
                            if (!grid.cells.contains(std::make_pair(x, y))) continue;

                            auto pair = std::make_pair(x, y);
                            flecs::entity neighbour_cell = grid.cells[pair];


                            m_cell_query[std::make_pair(cell.x, cell.y)].each([&](
                            flecs::iter &self_it, size_t self_i, const core::Position2D &pos,
                            const Collider &collider) {
                                    flecs::entity self = self_it.entity(self_i);
                                    m_cell_query[pair].each([&](flecs::iter &other_it, size_t other_i,
                                                                const core::Position2D &other_pos,
                                                                const Collider &other_collider) {
                                        flecs::entity other = other_it.entity(other_i);
                                        if (other.id() <= self.id()) return;

                                        if ((collider.collision_filter & other_collider.collision_type) == none) return;

                                        Rectangle self_rec = {
                                            pos.value.x + collider.bounds.x, pos.value.y + collider.bounds.y,
                                            collider.bounds.width,
                                            collider.bounds.height
                                        };
                                        Rectangle other_rec = {
                                            other_pos.value.x + other_collider.bounds.x,
                                            other_pos.value.y + other_collider.bounds.y,
                                            other_collider.bounds.width, other_collider.bounds.height
                                        };

                                        if (CheckCollisionRecs(self_rec, other_rec)) {
                                            collisions.push_back({self, other});
                                        }
                                    });
                                });
                        }
                    }
                    if (collisions.empty()) return;
                    systems::list_mutex.lock();
                    list.records.insert(list.records.end(), collisions.begin(), collisions.end());
                    systems::list_mutex.unlock();
                });
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
