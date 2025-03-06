//
// Created by Laurent Voisard on 12/22/2024.
//

#include "gui_module.h"

#include "components.h"
#define RAYGUI_IMPLEMENTATION
#include <modules/engine/rendering/pipeline_steps.h>
#include <stack>
#include <unordered_set>

#include "raygui.h"
#include "modules/engine/core/core_module.h"

namespace rendering::gui {
    void GUIModule::register_components(flecs::world &world) {
        world.component<Button>();
        world.component<Text>();
        world.component<Outline>();
    }

    void GUIModule::register_systems(flecs::world &world) {
        world.system().kind(flecs::OnStart).run([](flecs::iter &iter) {
            GuiLoadStyle("../resources/styles/amber/style_amber.rgs");
        });

        world.system("On start set move gui elements to match anchors")
                .kind(flecs::OnStart)
                .run([world](flecs::iter &iter) {
                    world.lookup("gui_canvas").set<Rectangle>({
                        0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
                    });
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
                        case HORIZONTAL_ANCHOR::CENTER:
                            temp.x = anchor->position.x + parent.x + parent.width / 2;
                            break;
                        case HORIZONTAL_ANCHOR::RIGHT:
                            temp.x = anchor->position.x + parent.x + parent.width;
                            break;
                        default:
                            temp.x = anchor->position.x + parent.x;
                            break;
                    }
                    switch (anchor->vertical_anchor) {
                        case VERTICAL_ANCHOR::MIDDLE:
                            temp.y = anchor->position.y + parent.y + parent.height / 2;
                            break;
                        case VERTICAL_ANCHOR::BOTTOM:
                            temp.y = anchor->position.y + parent.y + parent.height;
                            break;
                        default:
                            temp.y = anchor->position.y + parent.y;
                            break;
                    }
                    e.set<Rectangle>({temp});
                });

        world.system("Window Resized")
                .kind<PreRender>()
                .run([world](flecs::iter &iter) {
                    if (IsWindowResized()) {
                        world.lookup("gui_canvas").set<Rectangle>({
                            0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
                        });
                    }
                });

        world.system<const Button, const Rectangle>("Draw Button")
                .kind<RenderGUI>()
                .each([](const Button &button, const Rectangle &rect) {
                    if (GuiButton(rect, button.text.c_str())) {
                        button.on_click_system.run();
                    }
                });

        world.system<const Panel, const Rectangle>("Draw Panel")
                .kind<RenderGUI>()
                .each([](const Panel &panel, const Rectangle &rect) {
                    GuiPanel(rect, panel.name.c_str());
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


        world.system("Draw FPS").kind<RenderGUI>().run([](flecs::iter &iter) { DrawFPS(10, 10); });
    }
} // namespace rendering::gui
