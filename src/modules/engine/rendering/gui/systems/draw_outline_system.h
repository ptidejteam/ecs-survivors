//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_OUTLINE_SYSTEM_H
#define DRAW_OUTLINE_SYSTEM_H
#include <raylib.h>

#include "modules/engine/rendering/gui/components.h"

namespace rendering::gui::systems {
    inline void draw_outline_system(const Rectangle &rect, const Outline &outline) {
        GuiDrawRectangle(rect, outline.border_size, outline.border_color, outline.fill_color);
    }
}
#endif //DRAW_OUTLINE_SYSTEM_H
