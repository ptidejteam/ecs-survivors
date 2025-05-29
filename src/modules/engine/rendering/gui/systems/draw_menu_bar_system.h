//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_MENU_BAR_SYSTEM_H
#define DRAW_MENU_BAR_SYSTEM_H

#include <raylib.h>
#include "modules/engine/rendering/gui/components.h"

namespace rendering::gui {
    inline void draw_menu_bar_system(MenuBar &bar) {
        GuiDrawRectangle(
            {0, 0, (float) GetScreenWidth(), (float) 25},
            bar.border_size,
            GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)),
            GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    }
}
#endif //DRAW_MENU_BAR_SYSTEM_H
