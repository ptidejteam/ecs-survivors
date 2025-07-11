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
#include "modules/engine/input/components.h"
#include "modules/gameplay/components.h"
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
        world.system().kind(flecs::OnStart).run(systems::load_style_system);

        world.system<core::GameSettings>("On start set move gui elements to match anchors")
                .kind(flecs::OnStart)
                .with(flecs::Disabled).optional()
                .each(systems::set_gui_canvas_size_system);

        world.system<const Rectangle, Anchor>("on start, set anchored position")
                .kind(flecs::OnStart)
                .with(flecs::Disabled).optional()
                .each(systems::set_anchored_position_system);

        world.observer<const Rectangle>("parent rectangle changed enabled")
                .term_at(0).up()
                .event(flecs::OnSet)
                .each(systems::on_parent_rectangle_changed_observer);

        world.observer<const Rectangle>("parent rectangle changed disabled")
                .term_at(0).parent()
                .event(flecs::OnSet)
                .with(flecs::Disabled).filter()
                .each([](flecs::entity e, const Rectangle &rec) {
                    // some rly wierd shenanigans with disabled entities and observers i think
                    systems::on_parent_rectangle_changed_observer(e, rec);
                    e.children([&](flecs::entity c) {
                        systems::on_parent_rectangle_changed_observer(c, e.get<Rectangle>());
                    });
                });

        world.observer()
                .event(flecs::OnAdd)
                .with(flecs::Disabled)
                .each([](flecs::entity e) {
                    e.children([](flecs::entity child) {
                        if (child.has<Rectangle>())
                            child.disable();
                    });
                });
        world.observer()
                .event(flecs::OnRemove)
                .with(flecs::Disabled)
                .each([](flecs::entity e) {
                    e.children([](flecs::entity child) {
                        if (child.has<Rectangle>())
                            child.enable();
                    });
                });


        world.system<core::GameSettings>("Window Resized")
                .kind(flecs::PreFrame)
                .each(systems::check_window_resized_system);


        world.system<const Panel, const Rectangle>("Draw Panel")
                .kind<RenderGUI>()
                .each(systems::draw_panel_system);

        world.system<const Button, const Rectangle>("Draw Button")
                .kind<RenderGUI>()
                .each(systems::draw_button_system);

        world.system<const Text, const Rectangle>("Draw Text")
                .kind<RenderGUI>()
                .each(systems::draw_text_system);

        world.system<const Rectangle, const Outline>("Draw Outline")
                .kind<RenderGUI>()
                .each(systems::draw_outline_system);

        world.system<const Rectangle, ProgressBar>("Draw Progress bar")
                .kind<RenderGUI>()
                .each([](const Rectangle &rec, ProgressBar bar) {
                    GuiProgressBar(rec, std::to_string((int) bar.current_val).c_str(),
                                   std::to_string((int) bar.max_val).c_str(),
                                   &bar.current_val, bar.min_val, bar.max_val);
                });

        world.system<MenuBar>("Draw Menu Bar")
                .kind<RenderGUI>()
                .each(systems::draw_menu_bar_system);

        world.system<MenuBarTab, MenuBar>("Draw Tabs")
                .term_at(1).parent()
                .kind<RenderGUI>()
                .each(systems::draw_menu_bar_tab_system);

        world.system<MenuBarTabItem, MenuBarTab, Rectangle>("Draw Tab Items")
                .term_at(1).parent()
                .term_at(2).parent()
                .kind<RenderGUI>()
                .each(systems::draw_menu_bar_tab_item_system);
    }

    void GUIModule::register_entities(flecs::world &world) {
        gui_canvas = world.entity("gui_canvas").set<Rectangle>({
            0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
        });


        menu_bar = world.entity("menu_bar")
                .set<MenuBar>({
                    200,
                    1,
                    GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)),
                    GetColor(GuiGetStyle(BUTTON, BACKGROUND_COLOR)),
                });

        exp_panel = world.entity("exp_panel").child_of(gui_canvas)
                .set<Panel>({"exp_panel"})
                .set<Rectangle>({-250, -60, 500, 60})
                .set<Anchor>({CENTER, BOTTOM});

        exp_bar = world.entity("exp_bar").child_of(exp_panel)
                .set<ProgressBar>({0, 100, 0})
                .set<Rectangle>({-200, -30, 400, 20})
                .set<Anchor>({CENTER, BOTTOM});

        exp_level_txt = world.entity("exp_level_txt").child_of(exp_panel)
                .set<Text>({"Level: 1", TEXT_ALIGN_CENTER})
                .set<Rectangle>({-37.5, 10, 75, 20})
                .set<Anchor>({CENTER, TOP});

        pause_menu = m_world.entity("pause_menu").child_of(gui_canvas)
                .set<Panel>({"pause"})
                .set<Rectangle>({-150, -200, 300, 400})
                .set<Anchor>({CENTER, MIDDLE})
                .add<core::PauseOnEnabled>()
                .disable();

        auto input_toggle = pause_menu.child().add<input::InputToggleEnable>();
        input_toggle.child().set<input::KeyBinding>({KEY_ESCAPE, 0});

        level_up_menu = m_world.entity("level_up_menu").child_of(gui_canvas)
                .set<Panel>({"level up"})
                .set<Rectangle>({-300, -200, 600, 400})
                .set<Anchor>({CENTER, MIDDLE})
                .add<core::PauseOnEnabled>();

        world.entity().child_of(level_up_menu).set_name("level up menu text")
                .set<Rectangle>({-150, 5, 300, 50})
                .set<Anchor>({CENTER, TOP})
                .set<Text>({"You Leveled Up, Pick an upgrade", TEXT_ALIGN_CENTER});

        auto container = world.entity().child_of(level_up_menu).set_name("level up options container")
                .set<Rectangle>({-175, -150, 350, 300})
                .set<Anchor>({CENTER, MIDDLE})
                .set<Outline>({1, GRAY, Fade(WHITE, 0)});

            container.child().set_name("Option 1")
                .set<Rectangle>({-162.5, 5, 325, 85})
                .set<Anchor>({CENTER, TOP})
                .set<Button>({
                    "Option 1", world.system().kind(0).run([&](flecs::iter &it) { level_up_menu.disable(); })
                });
            container.child().set_name("Option 2")
                .set<Rectangle>({-162.5, -42.5, 325, 85})
                .set<Anchor>({CENTER, MIDDLE})
                .set<Button>({
                    "Option 2", world.system().kind(0).run([&](flecs::iter &it) { level_up_menu.disable(); })
                });
        container.child().set_name("Option 3")
                .set<Rectangle>({-162.5, -90, 325, 85})
                .set<Anchor>({CENTER, BOTTOM})
                .set<Button>({
                    "Option 3", world.system().kind(0).run([&](flecs::iter &it) { level_up_menu.disable(); })
                });

        level_up_menu.disable();
    }
} // namespace rendering::gui
