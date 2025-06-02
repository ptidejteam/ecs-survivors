//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_MENU_BAR_TAB_SYSTEM_H
#define DRAW_MENU_BAR_TAB_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

#include "modules/engine/rendering/gui/components.h"

namespace rendering::gui::systems {
    inline void draw_menu_bar_tab_system(flecs::iter &it, size_t i, MenuBarTab &tab, MenuBar &bar) {
        Rectangle rec = {
            GetScreenWidth() - (float) (i + 1) * bar.item_width, 0, (float) bar.item_width, (float) 25
        };

        if(!it.entity(i).has<Rectangle>()) {
            it.entity(i).set<Rectangle>({rec});
        }

        if (GuiButton(rec, tab.name.c_str())) {
            tab.active = !tab.active;
            it.entity(i).set<Rectangle>(rec);
        }
    }
}
#endif //DRAW_MENU_BAR_TAB_SYSTEM_H
