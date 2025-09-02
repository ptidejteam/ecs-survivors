//
// Created by laurent on 8/30/25.
//

#ifndef GUI_REGISTER_ENTITIES_SYSTEM_H
#define GUI_REGISTER_ENTITIES_SYSTEM_H
#include <flecs.h>
#include <raylib.h>

#include "gui/gui_module.h"
#include "gui/components.h"
namespace gui::systems {
    inline void register_entities_system(flecs::iter& iter) {
        auto world = iter.world();
        auto panel_texture = LoadTexture("assets/panel-010.png");
        auto button_texture = LoadTexture("assets/panel-009.png");
        GUIModule::panel_prefab = world.prefab().set<Panel>({
            panel_texture,
            {{0, 0, (float) panel_texture.width, (float) panel_texture.height}, 16, 16, 16, 16, NPATCH_NINE_PATCH}
        });

        GUIModule::button_prefab = world.prefab()
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


        GUIModule::gui_canvas = world.entity("gui_canvas").set<Rectangle>({
            0, 0, static_cast<float>(world.get<rendering::Settings>().window_height)
        });


        GUIModule::menu_bar = world.entity("menu_bar")
                .set<MenuBar>({
                    200,
                    1,
                    GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)),
                    GetColor(GuiGetStyle(BUTTON, BACKGROUND_COLOR)),
                });

    }
}
#endif // ECS_SURVIVORS_REGISTER_ENTITIES_SYSTEM_H
