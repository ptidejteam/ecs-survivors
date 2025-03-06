//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include <raygui.h>
#include <raylib.h>

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


    private:
        void register_components(flecs::world &world);

        void register_systems(flecs::world &world);

        static Color font_color() { return GetColor(GuiGetStyle(TEXTBOX, TEXT_COLOR_NORMAL)); }


        friend class BaseModule<GUIModule>;
    };
}

#endif //GUI_MODULE_H
