//
// Created by Laurent Voisard on 12/22/2024.
//

#include "gui_module.h"

#include "components.h"
#define RAYGUI_IMPLEMENTATION
#include <modules/engine/rendering/pipeline_steps.h>

#include "raygui.h"

namespace rendering::gui {
    void GUIModule::register_components(flecs::world &world) {
        world.component<Button>();
        world.component<OutlineDecorator>();
        world.component<CenteredDecorator>();
    }

    void GUIModule::register_systems(flecs::world &world) {
        world.system()
                .kind(flecs::OnStart)
                .run([](flecs::iter &iter) {
                    GuiLoadStyle("../resources/styles/dark/style_dark.rgs");
                });

        world.system<const Button>("Draw Button")
                .kind<RenderGUI>()
                .each([](const Button &button) {
                    if (GuiButton(button.rect, button.text.c_str())) {
                        button.onclick_system.run();
                    }
                });

        world.system<const Text>("Draw Text")
                .kind<RenderGUI>()
                .each([](const Text &text) {
                    GuiDrawText(text.text.c_str(), text.rect, 0, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
                });

        world.system("Draw FPS")
                .kind<RenderGUI>()
                .run([](flecs::iter &iter) {
                    DrawFPS(10, 10);
                });
    }
}
