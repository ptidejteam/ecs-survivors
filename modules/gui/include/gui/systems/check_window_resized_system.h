//
// Created by laurent on 29/05/25.
//

#ifndef CHECK_WINDOW_RESIZED_SYSTEM_H
#define CHECK_WINDOW_RESIZED_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

#include "set_gui_canvas_size_system.h"
#include "core/components.h"

namespace rendering::gui::systems {
    inline void check_window_resized_system(flecs::entity e, core::GameSettings &settings) {
        if (IsWindowResized()) {
            set_gui_canvas_size_system(settings);
#ifdef EMSCRIPTEN
                        SetMouseScale(settings.window_width / (float) settings.initial_width,
                                      settings.window_height / (float) settings.initial_height);
#endif
        }
    }
}
#endif //CHECK_WINDOW_RESIZED_SYSTEM_H
