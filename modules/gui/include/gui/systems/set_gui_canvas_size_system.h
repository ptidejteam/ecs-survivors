//
// Created by laurent on 29/05/25.
//

#ifndef SET_GUI_CANVAS_SIZE_SYSTEM_H
#define SET_GUI_CANVAS_SIZE_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

#include "rendering/components.h"
#include "gui/gui_module.h"

namespace gui::systems {
    inline void set_gui_canvas_size_system(flecs::iter& it, size_t, rendering::Viewport& vp) {
        std::cout << "world updated" << std::endl;
        GUIModule::gui_canvas.set<Rectangle>({
            0, 0, vp.rect.width, vp.rect.height
        });
    }
}
#endif //SET_GUI_CANVAS_SIZE_SYSTEM_H
