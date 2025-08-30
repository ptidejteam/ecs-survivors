//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_PANEL_SYSTEM_H
#define DRAW_PANEL_SYSTEM_H
#include "gui/components.h"

namespace gui::systems {
    inline void draw_panel_system(const Panel &panel, const Rectangle &rect) {
        // GuiDrawRectangle(
        //     rect,
        //     1,
        //     GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)),
        //     Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), 0.9f));

        DrawTextureNPatch(panel.texture, panel.info, rect, {0,0}, 0, ColorAlpha(BLACK, 0.8));

    }
}

#endif //DRAW_PANEL_SYSTEM_H
