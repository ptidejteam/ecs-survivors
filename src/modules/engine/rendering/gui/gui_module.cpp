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
