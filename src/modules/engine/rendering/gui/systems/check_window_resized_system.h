//
// Created by laurent on 29/05/25.
//

#ifndef CHECK_WINDOW_RESIZED_SYSTEM_H
#define CHECK_WINDOW_RESIZED_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

#include "set_gui_canvas_size_system.h"
#include "modules/engine/core/components.h"

namespace rendering::gui {
    inline void check_window_resized_system(const flecs::world &world, core::GameSettings &settings) {
        if (IsWindowResized()) {
            set_gui_canvas_size_system(world, settings);
#ifdef EMSCRIPTEN
                        SetMouseScale(settings.windowWidth / (float) settings.initialWidth,
                                      settings.windowHeight / (float) settings.initialHeight);
#endif
        }
    }
}
#endif //CHECK_WINDOW_RESIZED_SYSTEM_H
