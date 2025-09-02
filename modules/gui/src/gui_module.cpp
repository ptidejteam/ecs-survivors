//
// Created by Laurent Voisard on 12/22/2024.
//

#include "gui/gui_module.h"

#include <flecs.h>
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "core/components.h"
#include "gui/components.h"
#include "rendering/pipeline_steps.h"
#include "gui/systems/draw_interactable_textured_element_system.h"
#include "gui/systems/draw_menu_bar_system.h"
#include "gui/systems/draw_menu_bar_tab_item_system.h"
#include "gui/systems/draw_menu_bar_tab_system.h"
#include "gui/systems/draw_outline_system.h"
#include "gui/systems/draw_panel_system.h"
#include "gui/systems/draw_text_system.h"
#include "gui/systems/load_style_system.h"
#include "gui/systems/parent_rectangle_changed_observer.h"
#include "gui/systems/set_anchored_position_system.h"
#include "gui/systems/set_gui_canvas_size_system.h"
#include "gui/systems/parent_rectangle_changed_disabled_observer.h"

#include "gui/systems/disable_children_on_disable_system.h"
#include "gui/systems/enable_children_on_enable_system.h"
#include "gui/systems/invoke_button_callback_system.h"
#include "gui/systems/draw_progress_bar_system.h"

#include "gui/systems/interactable_transition_to_hovered_system.h"
#include "gui/systems/interactable_transition_to_normal_system.h"
#include "gui/systems/interactable_transition_to_pressed_system.h"
#include "gui/systems/interactable_transition_to_released_system.h"
#include "gui/systems/register_entities_system.h"
#include "rendering/rendering_module.h"

namespace gui {
    void GUIModule::register_components(flecs::world &world) {
        world.component<Text>();
        world.component<Outline>();
        world.component<Font>();
        world.set<FontAtlas>({
            std::unordered_map<int, Font>{
                {FONT_SIZE_16, LoadFontEx("../../assets/Spectral-SemiBold.ttf", FONT_SIZE_16, nullptr, 0)},
                {FONT_SIZE_32, LoadFontEx("../../assets/Spectral-SemiBold.ttf", FONT_SIZE_32, nullptr, 0)},
                {FONT_SIZE_48, LoadFontEx("../../assets/Spectral-SemiBold.ttf", FONT_SIZE_48, nullptr, 0)},
                {FONT_SIZE_64, LoadFontEx("../../assets/Spectral-SemiBold.ttf", FONT_SIZE_64, nullptr, 0)},
            }
        });
    }

    void GUIModule::register_systems(flecs::world &world) {
        world.system().kind(flecs::OnStart).run(systems::load_style_system);

        world.system<rendering::Viewport>("On start set move gui elements to match anchors")
                .kind(flecs::OnStart)
                .each(systems::set_gui_canvas_size_system);

        world.system<const Rectangle, Anchor>("on start, set anchored position")
                .kind(flecs::OnStart)
                .with(flecs::Disabled).optional()
                .each(systems::set_anchored_position_system);

        world.observer<rendering::Viewport>()
                .event(flecs::OnSet)
                .each(systems::set_gui_canvas_size_system);

        world.observer<rendering::ScreenViewport>()
            .event(flecs::OnSet)
            .each(systems::set_gui_canvas_size_system);

        world.observer<rendering::VirtualViewport>()
            .event(flecs::OnSet)
            .each(systems::set_gui_canvas_size_system);

        // TODO: CRITICAL, need to only update when the screen reso changes, not every frame
        // Observer is acting weird so I canged to system
        world.system<const Rectangle>("parent rectangle changed enabled")
                .term_at(0).up()
                //.event(flecs::OnSet)
                .each(systems::on_parent_rectangle_changed_observer);

        // TODO: CRITICAL, need to only update when the screen reso changes, not every frame
        // Observer is acting weird so I canged to system
        world.system<const Rectangle>("parent rectangle changed disabled")
                .term_at(0).parent()
                //.event(flecs::OnSet)
                .with(flecs::Disabled).filter()
                .each(systems::on_parent_rectangle_changed_disabled_observer);

        world.observer()
                .event(flecs::OnAdd)
                .with(flecs::Disabled)
                .each(systems::disable_children_on_disable_system);

        world.observer()
                .event(flecs::OnRemove)
                .with(flecs::Disabled)
                .each(systems::enable_children_on_enable_system);

        world.system<const Rectangle>()
                .with<InteractableElementState>(Normal)
                .kind<rendering::RenderGUI>()
                .each(systems::interactable_transition_to_hovered_system);

        world.system<const Rectangle>()
                .with<InteractableElementState>(Hovered)
            .kind<rendering::RenderGUI>()
                .each(systems::interactable_transition_to_pressed_system);

        world.system<const Rectangle>()
                .with<InteractableElementState>(Pressed)
        .kind<rendering::RenderGUI>()
                .each(systems::interactable_transition_to_released_system);

        world.system<const Panel, const Rectangle>("Draw Panel")
                .kind<rendering::RenderGUI>()
                .each(systems::draw_panel_system);

        world.system<const TexturedElement, const InteractableElement, const Rectangle>("Draw textured interactable")
                .with<InteractableElementState>(flecs::Wildcard)
                .kind<rendering::RenderGUI>()
                .each(systems::draw_interactable_textured_element_system);

        world.system<const Text, const Rectangle, const InteractableElement*, const FontAtlas>("Draw Text")
                .kind<rendering::RenderGUI>()
                .term_at(3).singleton()
                .each(systems::draw_text_system);

        world.system<const Rectangle, const Outline>("Draw Outline")
                .kind<rendering::RenderGUI>()
                .each(systems::draw_outline_system);

        world.system<const Rectangle, ProgressBar>("Draw Progress bar")
                .kind<rendering::RenderGUI>()
                .each(systems::draw_progress_bar_system);

        world.system<MenuBar, Rectangle>("Draw Menu Bar")
                .term_at(1).parent()
                .kind<rendering::RenderGUI>()
                .each(systems::draw_menu_bar_system);

        world.system<MenuBarTab, MenuBar, Rectangle>("Draw Tabs")
                .term_at(1).parent()
                .term_at(2).parent()
                .kind<rendering::RenderGUI>()
                .each(systems::draw_menu_bar_tab_system);

        world.system<MenuBarTabItem, MenuBarTab, Rectangle>("Draw Tab Items")
                .term_at(1).parent()
                .term_at(2).parent()
                .kind<rendering::RenderGUI>()
                .each(systems::draw_menu_bar_tab_item_system);

        world.system<const ButtonCallback>("On Button clicked")
                .with<InteractableElementState>(Released)
                .kind<rendering::RenderGUI>()
                .each(systems::invoke_button_callback_system);

        world.system<const Rectangle>()
                .with<InteractableElementState>(Released)
                .kind(flecs::PostFrame)
                .each(systems::interactable_transition_to_normal_system);
    }

    void GUIModule::register_entities(flecs::world &world) {
        auto panel_texture = LoadTexture("../../assets/panel-010.png");
        auto button_texture = LoadTexture("../../assets/panel-009.png");
        panel_prefab = world.prefab().set<Panel>({
            panel_texture,
            {{0, 0, (float) panel_texture.width, (float) panel_texture.height}, 16, 16, 16, 16, NPATCH_NINE_PATCH}
        });

        button_prefab = world.prefab()
                .set<TexturedElement>({
                    button_texture,
                    {
                        {0, 0, (float) panel_texture.width, (float) panel_texture.height}, 16, 16, 16, 16,
                        NPATCH_NINE_PATCH
                    }
                })
                .set<InteractableElement>({
                    ColorAlpha(WHITE, 0.8),
                    ColorAlpha(BLACK, 1),
                    ColorAlpha(WHITE, 1)
                })
                .set<ButtonCallback>({
                    [] { std::cout << "You clicked me" << std::endl; }
                })
                .set<Text>({"Click me", FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
                .add<InteractableElementState>(Normal);


        //auto w = world.get<rendering::Settings>().window_width;
        //auto h = world.get<rendering::Settings>().window_height;
        gui_canvas = world.entity("gui_canvas").child_of(rendering::RenderingModule::main_viewport).set<Rectangle>({
            0, 0, static_cast<float>(1920), static_cast<float>(1080)
        });


        menu_bar = world.entity("menu_bar")
                .child_of(gui_canvas)
                .set<MenuBar>({
                    200,
                    1,
                    GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)),
                    GetColor(GuiGetStyle(BUTTON, BACKGROUND_COLOR)),
                }).disable();


    }
} // namespace gui
