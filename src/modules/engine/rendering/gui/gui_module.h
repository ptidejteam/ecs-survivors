//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include <raylib.h>
#include <raygui.h>

#include "components.h"
#include "modules/base_module.h"
#include "flecs.h"
#include "modules/engine/core/core_module.h"

namespace rendering::gui {
    class GUIModule : public BaseModule<GUIModule> {
    public:
        // do not add implementation to the constructor
        GUIModule(flecs::world &world): BaseModule(world) {
        };


        inline static flecs::entity gui_canvas;
        inline static flecs::entity menu_bar;
        inline static flecs::entity exp_panel;
        inline static flecs::entity exp_bar;
        inline static flecs::entity exp_level_txt;
        inline static flecs::entity pause_menu;
        inline static flecs::entity level_up_menu;

        static Color font_color() { return GetColor(GuiGetStyle(TEXTBOX, TEXT_COLOR_NORMAL)); }

    private:
        void register_components(flecs::world &world);

        void register_systems(flecs::world &world);

        void register_entities(flecs::world &world);


        friend class BaseModule<GUIModule>;
    };
}

#endif //GUI_MODULE_H
