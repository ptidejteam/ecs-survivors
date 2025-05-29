//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_TEXT_SYSTEM_H
#define DRAW_TEXT_SYSTEM_H

#include <raylib.h>
#include "modules/engine/rendering/gui/components.h"
#include "modules/engine/rendering/gui/gui_module.h"

namespace rendering::gui {
    inline void draw_text_system(const Text &text, const Rectangle &rect) {
        GuiDrawText(text.text.c_str(), rect, text.alignment, GUIModule::font_color());
    }
}
#endif //DRAW_TEXT_SYSTEM_H
