//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_TEXT_SYSTEM_H
#define DRAW_TEXT_SYSTEM_H

#include <raylib.h>
#include "gui/components.h"
#include "gui/gui_module.h"

namespace gui::systems {
    inline void draw_text_system(flecs::entity e, const Text &text, const Rectangle &rect, const FontAtlas &fonts) {
        Vector2 pos;
        Vector2 text_size = MeasureTextEx(fonts.fonts.at(text.font_size), text.text.c_str(), text.font_size, 0);
        switch (text.alignment) {
            case TEXT_ALIGN_CENTER:
                pos.x = rect.x + (rect.width - text_size.x) / 2;
                pos.y = rect.y + (rect.height - text_size.y) / 2;
                break;
            case TEXT_ALIGN_RIGHT:
                pos.x = rect.x + rect.width - text_size.x;
                pos.y = rect.y + rect.height - text_size.y;
                break;
            default:
                pos = {rect.x, rect.y};
                break;
        }

        DrawTextEx(fonts.fonts.at(text.font_size), text.text.c_str(), pos, text.font_size, 0, text.color);
    }


} // namespace gui::systems
#endif // DRAW_TEXT_SYSTEM_H
