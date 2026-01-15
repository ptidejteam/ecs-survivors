//
// Created by Laurent on 4/18/2025.
//

#include "debugging/debug_module.h"

#include <rendering/pipeline_steps.h>

#include "core/components.h"
#include "physics/components.h"
#include "rendering/components.h"
#include "gui/components.h"

#include "gui/gui_module.h"
#include <raymath.h>

#include "physics/physics_module.h"
#include "debugging/systems/debug_closest_enemy_to_player_system.h"
#include "debugging/systems/debug_collidable_entities_system.h"
#include "debugging/systems/debug_colliders_system.h"
#include "debugging/systems/debug_entity_count_system.h"
#include "debugging/systems/debug_fps_system.h"
#include "debugging/systems/debug_grid_system.h"
#include "debugging/systems/debug_mouse_position_system.h"


namespace debugging {
    void DebugModule::register_components(flecs::world &world) {
    }

    void DebugModule::register_systems(flecs::world &world) {
        debug_entity_ids = world.system<const core::Position2D, const physics::Collider>("Show Collidable ID")
                .kind<rendering::RenderObjects>()
                .each(systems::debug_collidable_entities_system);
        debug_entity_ids.disable();

        debug_collider_bounds = world.system<const core::Position2D, const physics::Collider>("Debug collider bounds")
                .kind<rendering::RenderObjects>()
                .each(systems::debug_collider_bounds_system);
        debug_collider_bounds.disable();

        debug_circle_colliders = world.system<const physics::CircleCollider, const core::Position2D>(
                    "Debug circle colliders")
                .kind<rendering::RenderObjects>()
                .with<rendering::Visible>()
                .group_by<rendering::Priority>()
                .each(systems::debug_circle_colliders_system);
        debug_circle_colliders.disable();

        debug_square_colliders = world.system<const core::Position2D, const physics::Collider>(
                    "Debug square colliders")
                .kind<rendering::RenderObjects>()
                .with<physics::BoxCollider>()
                .group_by<rendering::Priority>()
                .each(systems::debug_box_colliders_system);
        debug_square_colliders.disable();

        debug_FPS = world.system("Draw FPS")
                .kind<rendering::RenderGUI>()
                .run(systems::debug_fps_system);
        debug_FPS.disable();

        debug_entity_count = world.system("Draw Entity Count")
                .kind<rendering::RenderGUI>()
                .run(systems::debug_entity_count_system);
        debug_entity_count.disable();

        debug_mouse_pos = world.system("Draw Mouse Position")
                .kind<rendering::RenderGUI>()
                .run(systems::debug_mouse_position_system);
        debug_mouse_pos.disable();

        debug_grid = world.system<physics::SpatialHashingGrid, physics::GridCell>("Draw Grid")
                .kind<rendering::RenderObjects>()
                .each(systems::debug_grid_system);
        debug_grid.disable();

        debug_closest_enemy = world.system("Draw Ray to closest target")
                .kind<rendering::RenderObjects>()
                .run(systems::debug_closest_enemy_to_player_system);
        debug_closest_enemy.disable();

            grid_cell_grow = world.system<physics::SpatialHashingGrid>()
            .kind(0)
            .each([] (flecs::entity e, physics::SpatialHashingGrid& grid) {
                    grid.cell_size = grid.cell_size + 16;
                    e.set<physics::SpatialHashingGrid>({grid});
            });
            grid_cell_grow.disable();

            grid_cell_shrink = world.system<physics::SpatialHashingGrid>()
            .kind(0)
            .each([] (flecs::entity e, physics::SpatialHashingGrid& grid) {
                    grid.cell_size = std::max(16, grid.cell_size - 16);
                    e.set<physics::SpatialHashingGrid>({grid});
            });
            grid_cell_shrink.disable();
    }

    void DebugModule::register_entities(flecs::world &world) {
        auto dropdown = world.entity("debug_dropdown").child_of(gui::GUIModule::menu_bar)
                .set<gui::MenuBarTab>({"Debug Tools", 25});

        world.entity("debug_collisions_item_0").child_of(dropdown)
                .set<gui::MenuBarTabItem>({
                    "Toggle Collidable Entity Ids", debug_entity_ids, gui::TOGGLE
                });
        world.entity("debug_collisions_item_1_0").child_of(dropdown)
                .set<gui::MenuBarTabItem>({
                    "Toggle Collider Bounds", debug_collider_bounds, gui::TOGGLE
                });
        world.entity("debug_collisions_item_1_1").child_of(dropdown)
                .set<gui::MenuBarTabItem>({
                    "Toggle Circle Colliders", debug_circle_colliders, gui::TOGGLE
                });
        world.entity("debug_collisions_item_1_2").child_of(dropdown)
                .set<gui::MenuBarTabItem>({
                    "Toggle Square Colliders", debug_square_colliders, gui::TOGGLE
                });
        world.entity("debug_collisions_item_2").child_of(dropdown)
                .set<gui::MenuBarTabItem>({"Toggle FPS", debug_FPS, gui::TOGGLE});
        world.entity("debug_collisions_item_3").child_of(dropdown)
                .set<gui::MenuBarTabItem>(
                    {"Toggle Entity Count", debug_entity_count, gui::TOGGLE});
        world.entity("debug_collisions_item_4").child_of(dropdown)
                .set<gui::MenuBarTabItem>({"Toggle Mouse Pos", debug_mouse_pos, gui::TOGGLE});
        world.entity("debug_collisions_item_5").child_of(dropdown)
                .set<gui::MenuBarTabItem>({"Toggle Grid", debug_grid, gui::TOGGLE});
        world.entity("debug_collisions_item_6").child_of(dropdown)
                .set<gui::MenuBarTabItem>({
                    "Toggle View Closest Enemy", debug_closest_enemy, gui::TOGGLE
                });

            world.entity("debug_collisions_item_7").child_of(dropdown)
                .set<gui::MenuBarTabItem>({"Toggle Naive Collisions", physics::m_collision_detection_naive_system, gui::TOGGLE});
            world.entity("debug_collisions_item_8").child_of(dropdown)
                    .set<gui::MenuBarTabItem>({
                        "Toggle Hashing Collisions", physics::m_collision_detection_spatial_hashing_system, gui::TOGGLE
                    });
            world.entity("debug_collisions_item_9").child_of(dropdown)
                   .set<gui::MenuBarTabItem>({
                       "Toggle Hashing Collisions ECS", physics::m_collision_detection_spatial_ecs, gui::TOGGLE
                   });
            world.entity("debug_collisions_item_10").child_of(dropdown)
                   .set<gui::MenuBarTabItem>({
                       "Grow Cell Size", grid_cell_grow, gui::RUN
                   });
            world.entity("debug_collisions_item_11").child_of(dropdown)
                   .set<gui::MenuBarTabItem>({
                       "Shrink Cell Size", grid_cell_shrink, gui::RUN
                   });
    }
}
