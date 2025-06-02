//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_PANEL_SYSTEM_H
#define DRAW_PANEL_SYSTEM_H
#include "modules/engine/rendering/gui/components.h"

namespace rendering::gui::systems {
    inline void draw_panel_system(const Panel &panel, const Rectangle &rect) {
        GuiPanel(rect, panel.name.c_str());
    }
}

#endif //DRAW_PANEL_SYSTEM_H
