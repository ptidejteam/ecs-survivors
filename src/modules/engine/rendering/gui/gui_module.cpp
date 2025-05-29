//
// Created by Laurent Voisard on 12/22/2024.
//

#include "gui_module.h"

#include <flecs.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "components.h"
#include <modules/engine/rendering/pipeline_steps.h>
#include "modules/engine/core/core_module.h"
#include "systems/check_window_resized_system.h"
#include "systems/draw_button_system.h"
#include "systems/draw_menu_bar_system.h"
#include "systems/draw_menu_bar_tab_item_system.h"
#include "systems/draw_menu_bar_tab_system.h"
#include "systems/draw_outline_system.h"
#include "systems/draw_panel_system.h"
#include "systems/draw_text_system.h"
#include "systems/load_style_system.h"
#include "systems/parent_rectangle_changed_observer.h"
#include "systems/set_anchored_position_system.h"
#include "systems/set_gui_canvas_size_system.h"

namespace rendering::gui {
    void GUIModule::register_components(flecs::world &world) {
        world.component<Button>();
        world.component<Text>();
        world.component<Outline>();
    }

    void GUIModule::register_systems(flecs::world &world) {
        world.system().kind(flecs::OnStart).run(load_style_system);

        world.system<core::GameSettings>("On start set move gui elements to match anchors")
                .kind(flecs::OnStart)
                .each([world](core::GameSettings &settings) {
                    set_gui_canvas_size_system(world, settings);
                });

        world.system<const Rectangle, Anchor>("on start, set anchored position")
                .kind(flecs::OnStart)
                .each(set_anchored_position_system);

        world.observer<const Rectangle>("parent rectangle changed")
                .term_at(0).parent()
                .event(flecs::OnSet)
                .each(on_parent_rectangle_changed_observer);

        world.system<core::GameSettings>("Window Resized")
                .kind<PreRender>()
                .each([world](core::GameSettings &settings) {
                    check_window_resized_system(world, settings);
                });

        world.system<const Panel, const Rectangle>("Draw Panel")
                .kind<RenderGUI>()
                .each(draw_panel_system);

        world.system<const Button, const Rectangle>("Draw Button")
                .kind<RenderGUI>()
                .each(draw_button_system);

        world.system<const Text, const Rectangle>("Draw Text")
                .kind<RenderGUI>()
                .each(draw_text_system);

        world.system<const Rectangle, const Outline>("Draw Outline")
                .kind<RenderGUI>()
                .each(draw_outline_system);

        world.system<MenuBar>("Draw Menu Bar")
                .kind<RenderGUI>()
                .each(draw_menu_bar_system);

        world.system<MenuBarTab, MenuBar>("Draw Tabs")
                .term_at(1).parent()
                .kind<RenderGUI>()
                .each(draw_menu_bar_tab_system);

        world.system<MenuBarTabItem, MenuBarTab, Rectangle>("Draw Tab Items")
                .term_at(1).parent()
                .term_at(2).parent()
                .kind<RenderGUI>()
                .each(draw_menu_bar_tab_item_system);
    }

    void GUIModule::register_entities(flecs::world &world) {
        menu_bar = world.entity("menu_bar")
                .set<MenuBar>({
                    200,
                    1,
                    GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)),
                    GetColor(GuiGetStyle(BUTTON, BACKGROUND_COLOR)),
                });
    }
} // namespace rendering::gui