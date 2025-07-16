//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_MENU_BAR_TAB_ITEM_SYSTEM_H
#define DRAW_MENU_BAR_TAB_ITEM_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

#include "modules/engine/rendering/gui/components.h"


namespace rendering::gui::systems {
    inline void draw_menu_bar_tab_item_system(flecs::iter &it, size_t i, MenuBarTabItem &item, MenuBarTab &tab, Rectangle &rec) {
        //std::cout << i << std::endl;
        if (!tab.active) return;
        if (GuiLabelButton({rec.x, rec.y + (i + 1) * rec.height, rec.width, rec.height}, item.name.c_str())) {
            // tab.active = !tab.active;
            if(item.type == TOGGLE) {
                if (item.toggle_system_entity.enabled())
                    item.toggle_system_entity.disable();
                else
                    item.toggle_system_entity.enable();
            } else if (item.type == RUN) {
                item.toggle_system_entity.run();
            }
        }
    }
}
#endif //DRAW_MENU_BAR_TAB_ITEM_SYSTEM_H
