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

#include "systems/debug_closest_enemy_to_player_system.h"
#include "systems/debug_colliders_system.h"
#include "systems/debug_entity_count_system.h"
#include "systems/debug_fps_system.h"
#include "systems/debug_grid_system.h"
#include "systems/debug_mouse_position_system.h"


namespace debug {
    void DebugModule::register_components(flecs::world &world) {
    }

    void DebugModule::register_systems(flecs::world &world) {
        debug_colliders = world.system<const physics::Collider, const core::Position2D>("Debug colliders")
                .kind<rendering::Render>()
                .with<rendering::Visible>()
                .group_by<rendering::Priority>()
                .each(systems::debug_colliders_system);
        debug_colliders.disable();

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

        debug_grid = world.system("Draw Grid")
                .kind<rendering::RenderGUI>()
                .run(systems::debug_grid_system);
        debug_grid.disable();

        debug_closest_enemy = world.system("Draw Ray to closest target")
                .kind<rendering::RenderGUI>()
                .run(systems::debug_closest_enemy_to_player_system);
        debug_closest_enemy.disable();
    }

    void DebugModule::register_entities(flecs::world &world) {
        auto dropdown = world.entity("debug_dropdown").child_of(rendering::gui::GUIModule::menu_bar)
                .set<rendering::gui::MenuBarTab>({"Debug Tools", 25});

        world.entity("debug_collisions_item_1").child_of(dropdown)
                .set<rendering::gui::MenuBarTabItem>({"Toggle Colliders", debug_colliders, rendering::gui::TOGGLE});
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
                .set<rendering::gui::MenuBarTabItem>({"Toggle View Closest Enemy", debug_closest_enemy, rendering::gui::TOGGLE});
    }
}
