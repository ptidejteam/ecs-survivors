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

namespace rendering::gui {
    void GUIModule::register_components(flecs::world &world) {
        world.component<Button>();
        world.component<Text>();
        world.component<Outline>();
    }

    void GUIModule::register_systems(flecs::world &world) {
        world.system().kind(flecs::OnStart).run([](flecs::iter &iter) {
            GuiLoadStyle("../resources/styles/dark/style_dark.rgs");
        });

        world.system<const Anchor, Rectangle, const Rectangle>("Add anchored position")
                .term_at(2).cascade()
                .kind(flecs::OnStart)
                .each([](flecs::entity e, const Anchor &anchor, Rectangle &rectangle, const Rectangle &parent) {
                    switch (anchor.horizontal_anchor) {
                        case HORIZONTAL_ANCHOR::CENTER:
                            rectangle.x = rectangle.x + parent.x + parent.width / 2;
                            break;
                        case HORIZONTAL_ANCHOR::RIGHT:
                            rectangle.x = rectangle.x + parent.x + parent.width;
                            break;
                        default:
                            rectangle.x = rectangle.x + parent.x;
                            break;
                    }
                    switch (anchor.vertical_anchor) {
                        case VERTICAL_ANCHOR::MIDDLE:
                            rectangle.y = rectangle.y + parent.y + parent.height / 2;
                            break;
                        case VERTICAL_ANCHOR::BOTTOM:
                            rectangle.y = rectangle.y + parent.y + parent.height;
                            break;
                        default:
                            rectangle.y = rectangle.y + parent.y;
                            break;
                    }
                });

        world.observer<const Anchor, Rectangle, const Rectangle>("reset on window changed anchored position")
                .term_at(2).cascade()
                .event(flecs::OnSet)
                .each([](flecs::entity e, const Anchor &anchor, Rectangle &rectangle, const Rectangle &parent) {
                    switch (anchor.horizontal_anchor) {
                        case HORIZONTAL_ANCHOR::CENTER:
                            rectangle.x = rectangle.x + parent.x + parent.width / 2;
                            break;
                        case HORIZONTAL_ANCHOR::RIGHT:
                            rectangle.x = rectangle.x + parent.x + parent.width;
                            break;
                        default:
                            rectangle.x = rectangle.x + parent.x;
                            break;
                    }
                    switch (anchor.vertical_anchor) {
                        case VERTICAL_ANCHOR::MIDDLE:
                            rectangle.y = rectangle.y + parent.y + parent.height / 2;
                            break;
                        case VERTICAL_ANCHOR::BOTTOM:
                            rectangle.y = rectangle.y + parent.y + parent.height;
                            break;
                        default:
                            rectangle.y = rectangle.y + parent.y;
                            break;
                    }
                });

        world.system<const Button, const Rectangle>("Draw Button")
                .kind<RenderGUI>()
                .each([](const Button &button, const Rectangle &rect) {
                    if (GuiButton(rect, button.text.c_str())) {
                        button.onclick_system.run();
                    }
                });

        world.system<const Panel, const Rectangle>()
                .kind<RenderGUI>()
                .each([](const Panel &panel, const Rectangle &rect) {
                    GuiPanel(rect, "Panel");
                });

        world.system<const Text, const Rectangle>("Draw Text")
                .kind<RenderGUI>()
                .each([](const Text &text, const Rectangle &rect) {
                    GuiDrawText(text.text.c_str(), rect, text.alignment, font_color());
                });

        world.system<const Rectangle, const Outline>()
                .kind<RenderGUI>()
                .each([](const Rectangle &rect, const Outline &outline) {
                    GuiDrawRectangle(rect, outline.border_size, outline.border_color, outline.fill_color);
                });


        world.system("Draw FPS").kind<RenderGUI>().run([](flecs::iter &iter) { DrawFPS(10, 10); });
    }
} // namespace rendering::gui
