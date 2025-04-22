//
// Created by Laurent Voisard on 12/22/2024.
//

#include "gui_module.h"

#include "components.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <modules/engine/rendering/pipeline_steps.h>

#include "game.h"
#include "raylib.h"
#include "modules/engine/core/core_module.h"
#include "modules/engine/rendering/components.h"

namespace rendering::gui {
    void GUIModule::register_components(flecs::world &world) {
        world.component<Button>();
        world.component<Text>();
        world.component<Outline>();
    }

    void GUIModule::register_systems(flecs::world &world) {
        world.system().kind(flecs::OnStart).run([](flecs::iter &iter) {
            std::cout << "Loading Style" << std::endl;
            GuiLoadStyle("../resources/styles/style_amber.rgs");
        });

        world.system<core::GameSettings>("On start set move gui elements to match anchors")
                .kind(flecs::OnStart)
                .each([world](core::GameSettings &settings) {
                    world.lookup("gui_canvas").set<Rectangle>({
                        0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
                    });
                    settings.windowHeight = GetScreenHeight();
                    settings.windowWidth = GetScreenWidth();
                });

        world.system<const Rectangle, Anchor>("on start, set anchored position")
                .kind(flecs::OnStart)
                .each([](const Rectangle &rectangle, Anchor &anchor) {
                    anchor.position.x = rectangle.x;
                    anchor.position.y = rectangle.y;
                });

        //
        world.observer<const Rectangle>("parent rectangle changed")
                .term_at(0).parent()
                .event(flecs::OnSet)
                .each([](flecs::entity e, const Rectangle &parent) {
                    if (!e.has<Anchor>()) return;
                    if (!e.has<Rectangle>()) return;

                    auto anchor = e.get<Anchor>();

                    Rectangle temp{*e.get<Rectangle>()};
                    switch (anchor->horizontal_anchor) {
                        case CENTER:
                            temp.x = anchor->position.x + parent.x + parent.width / 2;
                            break;
                        case RIGHT:
                            temp.x = anchor->position.x + parent.x + parent.width;
                            break;
                        default:
                            temp.x = anchor->position.x + parent.x;
                            break;
                    }
                    switch (anchor->vertical_anchor) {
                        case MIDDLE:
                            temp.y = anchor->position.y + parent.y + parent.height / 2;
                            break;
                        case BOTTOM:
                            temp.y = anchor->position.y + parent.y + parent.height;
                            break;
                        default:
                            temp.y = anchor->position.y + parent.y;
                            break;
                    }
                    e.set<Rectangle>({temp});
                });

        world.system<core::GameSettings>("Window Resized")
                .kind<PreRender>()
                .each([world](core::GameSettings &settings) {
                    if (IsWindowResized()) {
                        world.lookup("gui_canvas").set<Rectangle>({
                            0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
                        });
                        settings.windowHeight = GetScreenHeight();
                        settings.windowWidth = GetScreenWidth();
#ifdef EMSCRIPTEN
                        SetMouseScale(settings.windowWidth / (float) settings.initialWidth,
                                      settings.windowHeight / (float) settings.initialHeight);
#endif
                    }
                });

        world.system<const Panel, const Rectangle>("Draw Panel")
                .kind<RenderGUI>()
                .each([](const Panel &panel, const Rectangle &rect) {
                    GuiPanel(rect, panel.name.c_str());
                });

        world.system<const Button, const Rectangle>("Draw Button")
                .kind<RenderGUI>()
                .each([](const Button &button, const Rectangle &rect) {
                    GuiSetStyle(BUTTON, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
                    if (GuiButton(rect, button.text.c_str())) {
                        button.on_click_system.run();
                    }
                    GuiSetStyle(BUTTON, TEXT_WRAP_MODE, DEFAULT);
                });

        world.system<const Text, const Rectangle>("Draw Text")
                .kind<RenderGUI>()
                .each([](const Text &text, const Rectangle &rect) {
                    GuiDrawText(text.text.c_str(), rect, text.alignment, font_color());
                });

        world.system<const Rectangle, const Outline>("Draw Outline")
                .kind<RenderGUI>()
                .each([](const Rectangle &rect, const Outline &outline) {
                    GuiDrawRectangle(rect, outline.border_size, outline.border_color, outline.fill_color);
                });

        world.system<MenuBar>("Draw Menu Bar")
                .kind<RenderGUI>()
                .each([](MenuBar &bar) {
                    GuiDrawRectangle(
                        {0, 0, (float) GetScreenWidth(), (float) 25},
                        bar.border_size,
                        GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)),
                        GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                });

        world.system<MenuBarTab, MenuBar>("Draw Tabs")
                .term_at(1).parent()
                .kind<RenderGUI>()
                .each([](flecs::iter &it, size_t i, MenuBarTab &tab, MenuBar &bar) {
                    //std::cout << i << std::endl;
                    Rectangle rec = {
                        GetScreenWidth() - (float) (i + 1) * bar.item_width, 0, (float) bar.item_width, (float) 25
                    };

                    if (GuiButton(rec, tab.name.c_str())) {
                        tab.active = !tab.active;
                        it.entity(i).set<Rectangle>(rec);
                    }
                });

        world.system<MenuBarTabItem, MenuBarTab, Rectangle>("Draw Tab Items")
                .term_at(1).parent()
                .term_at(2).parent()
                .kind<RenderGUI>()
                .each([](flecs::iter &it, size_t i, MenuBarTabItem &item, MenuBarTab &tab, Rectangle &rec) {
                    //std::cout << i << std::endl;
                    if (!tab.active) return;
                    if (GuiButton({rec.x, rec.y + (i + 1) * rec.height, rec.width, rec.height}, item.name.c_str())) {
                        // tab.active = !tab.active;
                        if(item.type == TOGGLE) {
                            if (item.toggle_system_entity.enabled())
                                item.toggle_system_entity.disable();
                            else
                                item.toggle_system_entity.enable();
                        } else if (item.type == RUN) {
                            item.toggle_system_entity.run();
                        }
                    }
                });
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
