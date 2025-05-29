//
// Created by laurent on 29/05/25.
//

#ifndef SET_GUI_CANVAS_SIZE_SYSTEM_H
#define SET_GUI_CANVAS_SIZE_SYSTEM_H

#include "modules/engine/core/components.h"
#include <flecs.h>
#include <raylib.h>

namespace rendering::gui {
    inline void set_gui_canvas_size_system(const flecs::world& world, core::GameSettings& settings) {
        world.lookup("gui_canvas").set<Rectangle>({
            0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
        });
        settings.windowHeight = GetScreenHeight();
        settings.windowWidth = GetScreenWidth();
    }
}
#endif //SET_GUI_CANVAS_SIZE_SYSTEM_H
