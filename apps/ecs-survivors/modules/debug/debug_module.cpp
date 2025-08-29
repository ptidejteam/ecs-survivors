//
// Created by Laurent on 4/18/2025.
//

#include "debug_module.h"

#include <modules/engine/rendering/pipeline_steps.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/rendering/components.h"
#include "modules/engine/rendering/gui/components.h"

#include "modules/engine/rendering/gui/gui_module.h"
#include <raymath.h>

#include "modules/engine/physics/physics_module.h"
#include "systems/debug_closest_enemy_to_player_system.h"
#include "systems/debug_collidable_entities_system.h"
#include "systems/debug_colliders_system.h"
#include "systems/debug_entity_count_system.h"
#include "systems/debug_fps_system.h"
#include "systems/debug_grid_system.h"
#include "systems/debug_mouse_position_system.h"


namespace debug {
    void DebugModule::register_components(flecs::world &world) {
    }

    void DebugModule::register_systems(flecs::world &world) {
        debug_entity_ids = world.system<const core::Position2D, const physics::Collider>("Show Collidable ID")
                .kind<rendering::RenderGizmos>()
                .each(systems::debug_collidable_entities_system);
        debug_entity_ids.disable();

        debug_collider_bounds = world.system<const core::Position2D, const physics::Collider>("Debug collider bounds")
                .kind<rendering::RenderGizmos>()
                .each(systems::debug_collider_bounds_system);
        debug_collider_bounds.disable();

        debug_circle_colliders = world.system<const physics::CircleCollider, const core::Position2D>(
                    "Debug circle colliders")
                .kind<rendering::RenderGizmos>()
                .with<rendering::Visible>()
                .group_by<rendering::Priority>()
                .each(systems::debug_circle_colliders_system);
        debug_circle_colliders.disable();

        debug_square_colliders = world.system<const core::Position2D, const physics::Collider>(
                    "Debug square colliders")
                .kind<rendering::RenderGizmos>()
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

        debug_grid = world.system<rendering::TrackingCamera, physics::SpatialHashingGrid, physics::GridCell>("Draw Grid")
                .term_at(0).singleton()
                .term_at(1).singleton()
                .kind<rendering::RenderGizmos>()
                .each(systems::debug_grid_system);
        debug_grid.disable();

        debug_closest_enemy = world.system("Draw Ray to closest target")
                .kind<rendering::RenderGizmos>()
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
        auto dropdown = world.entity("debug_dropdown").child_of(rendering::gui::GUIModule::menu_bar)
                .set<rendering::gui::MenuBarTab>({"Debug Tools", 25});

        world.entity("debug_collisions_item_0").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Toggle Collidable Entity Ids", debug_entity_ids, rendering::gui::TOGGLE
                });
        world.entity("debug_collisions_item_1_0").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Toggle Collider Bounds", debug_collider_bounds, rendering::gui::TOGGLE
                });
        world.entity("debug_collisions_item_1_1").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Toggle Circle Colliders", debug_circle_colliders, rendering::gui::TOGGLE
                });
        world.entity("debug_collisions_item_1_2").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Toggle Square Colliders", debug_square_colliders, rendering::gui::TOGGLE
                });
        world.entity("debug_collisions_item_2").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Toggle FPS", debug_FPS, rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_3").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>(
                    {"Toggle Entity Count", debug_entity_count, rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_4").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Toggle Mouse Pos", debug_mouse_pos, rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_5").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Toggle Grid", debug_grid, rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_6").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({
                    "Toggle View Closest Enemy", debug_closest_enemy, rendering::gui::TOGGLE
                });

            world.entity("debug_collisions_item_7").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Toggle Naive Collisions", physics::m_collision_detection_naive_system, rendering::gui::TOGGLE});
            world.entity("debug_collisions_item_8").child_of(dropdown)
                    .set<rendering::gui::MenuBarTabItem>({
                        "Toggle Hashing Collisions", physics::m_collision_detection_spatial_hashing_system, rendering::gui::TOGGLE
                    });
            world.entity("debug_collisions_item_9").child_of(dropdown)
                   .set<rendering::gui::MenuBarTabItem>({
                       "Toggle Hashing Collisions ECS", physics::m_collision_detection_spatial_ecs, rendering::gui::TOGGLE
                   });
            world.entity("debug_collisions_item_10").child_of(dropdown)
                   .set<rendering::gui::MenuBarTabItem>({
                       "Grow Cell Size", grid_cell_grow, rendering::gui::RUN
                   });
            world.entity("debug_collisions_item_11").child_of(dropdown)
                   .set<rendering::gui::MenuBarTabItem>({
                       "Shrink Cell Size", grid_cell_shrink, rendering::gui::RUN
                   });
    }
}
