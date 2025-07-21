//
// Created by Laurent on 7/21/2025.
//

#ifndef DRAW_PROGRESS_BAR_SYSTEM_H
#define DRAW_PROGRESS_BAR_SYSTEM_H

#include <string>
#include <raylib.h>
#include "modules/engine/rendering/gui/gui_module.h"

namespace rendering::gui::systems {
    inline void draw_progress_bar_system(const Rectangle &rec, ProgressBar bar) {
        GuiProgressBar(rec, std::to_string((int) bar.current_val).c_str(),
                       std::to_string((int) bar.max_val).c_str(),
                       &bar.current_val, bar.min_val, bar.max_val);
    }
}
#endif //DRAW_PROGRESS_BAR_SYSTEM_H
