//
// Created by laurent on 29/05/25.
//

#ifndef SET_GUI_CANVAS_SIZE_SYSTEM_H
#define SET_GUI_CANVAS_SIZE_SYSTEM_H

#include "core/components.h"
#include <flecs.h>
#include <raylib.h>

#include "gui_game/gui_module.h"

namespace rendering::gui::systems {
    inline void set_gui_canvas_size_system(core::GameSettings& settings) {
        GUIModule::gui_canvas.set<Rectangle>({
            0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
        });
        settings.window_height = GetScreenHeight();
        settings.window_width = GetScreenWidth();
    }
}
#endif //SET_GUI_CANVAS_SIZE_SYSTEM_H
