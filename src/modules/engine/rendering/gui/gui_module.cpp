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
                .each([world](core::GameSettings& settings) {
                    if (IsWindowResized()) {
                        world.lookup("gui_canvas").set<Rectangle>({
                            0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
                        });
                        settings.windowHeight = GetScreenHeight();
                        settings.windowWidth = GetScreenWidth();
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



        world.system("Draw FPS")
            .kind<RenderGUI>()
            .run([](flecs::iter &iter) {
                DrawFPS(10, 10);
            });

        auto entity_count_query = world.query_builder<Renderable>().build();
        world.system("Draw Entity Count")
            .kind<RenderGUI>()
            .run([entity_count_query](flecs::iter &iter) {
                DrawText(std::string(std::to_string(entity_count_query.count()) + " entities").c_str(), 10, 30, 20, GREEN);
            });
        auto entity_visible_count_query = world.query_builder<Renderable>().with<Visible>().build();
        world.system("Draw Visible Entity Count")
            .kind<RenderGUI>()
            .run([entity_visible_count_query](flecs::iter &iter) {
                DrawText(std::string(std::to_string(entity_visible_count_query.count()) + " visible entities").c_str(), 10, 50, 20, GREEN);
            });
    }
} // namespace rendering::gui
