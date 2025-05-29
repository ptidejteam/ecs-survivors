//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_BUTTON_SYSTEM_H
#define DRAW_BUTTON_SYSTEM_H

#include <raylib.h>

#include "modules/engine/rendering/gui/components.h"


namespace rendering::gui {
    inline void draw_button_system(const Button &button, const Rectangle &rect) {
        GuiSetStyle(BUTTON, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
        if (GuiButton(rect, button.text.c_str())) {
            button.on_click_system.run();
        }
        GuiSetStyle(BUTTON, TEXT_WRAP_MODE, DEFAULT);
    }
}
#endif //DRAW_BUTTON_SYSTEM_H
