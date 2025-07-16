//
// Created by laurent on 29/05/25.
//

#ifndef LOAD_STYLE_SYSTEM_H
#define LOAD_STYLE_SYSTEM_H

#include <flecs.h>

namespace rendering::gui::systems {
    inline void load_style_system(flecs::iter &iter) {
        GuiLoadStyle("../resources/styles/style_amber.rgs");
        GuiSetFont(iter.world().get<FontAtlas>().fonts.at(FONT_SIZE_16));
    }
}
#endif //LOAD_STYLE_SYSTEM_H
