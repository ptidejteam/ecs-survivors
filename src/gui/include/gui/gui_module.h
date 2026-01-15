//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include <raylib.h>

#include "base/base_module.h"
#include "flecs.h"

namespace gui {
    class GUIModule : public base::BaseModule<GUIModule> {
    public:
        // do not add implementation to the constructor
        GUIModule(flecs::world &world): BaseModule(world) {
        };


        inline static flecs::entity gui_canvas;

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
