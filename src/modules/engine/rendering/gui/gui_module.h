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
        inline static flecs::entity button_prefab;
        inline static flecs::entity panel_prefab;
        inline static flecs::entity menu_bar;

        static Color font_color() { return LIGHTGRAY; }

    private:
        void register_components(flecs::world &world);

        void register_systems(flecs::world &world);

        void register_entities(flecs::world &world);


        friend class BaseModule<GUIModule>;
    };
}

#endif //GUI_MODULE_H
