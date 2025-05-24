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
#include "raygui.h"


namespace debug {
    void DebugModule::register_components(flecs::world& world) {
    }

    void DebugModule::register_systems(flecs::world& world) {

        debug_colliders = world.system<const physics::Collider, const core::Position2D>("Debug colliders")
                .kind<rendering::Render>()
                .with<rendering::Visible>()
                .group_by<rendering::Priority>()
                .each([](const physics::Collider &collider, const core::Position2D &position) {
                    DrawCircleLines(position.value.x, position.value.y, collider.radius, GREEN);
                });
        debug_colliders.disable();

        debug_FPS = world.system("Draw FPS")
                .kind<rendering::RenderGUI>()
                .run([](flecs::iter &iter) {
                    DrawRectangleRec({0, 10, 225, 20}, DARKGRAY);
                    DrawFPS(10, 10);
                });
        debug_FPS.disable();

        auto entity_count_query = world.query_builder<rendering::Renderable>().build();
        auto entity_visible_count_query = world.query_builder<rendering::Renderable>().with<rendering::Visible>().build();
        debug_entity_count = world.system("Draw Entity Count")
                .kind<rendering::RenderGUI>()
                .run([entity_count_query, entity_visible_count_query](flecs::iter &iter) {
                    DrawRectangleRec({0, 30, 225, 40}, DARKGRAY);
                    DrawText(std::string(std::to_string(entity_count_query.count()) + " entities").c_str(), 10, 30, 20,
                             GREEN);
                    DrawText(
                        std::string(std::to_string(entity_visible_count_query.count()) + " visible entities").c_str(),
                        10, 50, 20, GREEN);
                });
        debug_entity_count.disable();

        debug_mouse_pos = world.system("Draw Mouse Position")
                .kind<rendering::RenderGUI>()
                .run([](flecs::iter &iter) {
                    DrawCircle(GetMouseX(), GetMouseY(), 10, RED);
                });
        debug_mouse_pos.disable();

        debug_grid = world.system("Draw Grid")
                .kind<rendering::RenderGUI>()
                .run([](flecs::iter &iter) {
                    GuiGrid({0,0, (float)GetScreenWidth(), (float)GetScreenHeight()}, "grid", 32, 1, nullptr);
                });
        debug_grid.disable();
    }

    void DebugModule::register_entities(flecs::world &world) {
        auto dropdown = world.entity("debug_dropdown").child_of(rendering::gui::GUIModule::menu_bar)
            .set<rendering::gui::MenuBarTab>({"Debug Tools", 25});

        world.entity("debug_collisions_item_1").child_of(dropdown)
            .set<rendering::gui::MenuBarTabItem>({"Toggle Colliders", debug_colliders,rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_2").child_of(dropdown)
           .set<rendering::gui::MenuBarTabItem>({"Toggle FPS", debug_FPS, rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_3").child_of(dropdown)
           .set<rendering::gui::MenuBarTabItem>({"Toggle Entity Count", debug_entity_count, rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_4").child_of(dropdown)
           .set<rendering::gui::MenuBarTabItem>({"Toggle Mouse Pos", debug_mouse_pos, rendering::gui::TOGGLE});
        world.entity("debug_collisions_item_5").child_of(dropdown)
                   .set<rendering::gui::MenuBarTabItem>({"Toggle Grid", debug_grid, rendering::gui::TOGGLE});

    }

}
