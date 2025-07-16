//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_BUTTON_SYSTEM_H
#define DRAW_BUTTON_SYSTEM_H

#include <raylib.h>

#include "modules/engine/rendering/gui/components.h"


namespace rendering::gui::systems {
    inline void draw_button_system(const Button &button, const ButtonLabel &label, const ButtonCallback &callback,
                                   const Rectangle &rect, const FontAtlas &fonts) {
        Color tint = button.normal_tint;

        if (CheckCollisionPointRec(GetMousePosition(), rect)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                callback.callback();
                tint = button.selected_tint;
            } else {
                tint = button.hovered_tint;
            }
        }


        DrawTextureNPatch(button.texture, button.info, rect, {0, 0}, 0.0f, tint);
        Vector2 text_size = MeasureTextEx(fonts.fonts.at(label.font_size), label.label.c_str(), label.font_size, 0);
        DrawTextEx(fonts.fonts.at(label.font_size), label.label.c_str(), {
                       rect.x + (rect.width - text_size.x) / 2, rect.y + (rect.height - text_size.y) / 2,
                   }, label.font_size, 0, BLACK);
    }
}
#endif //DRAW_BUTTON_SYSTEM_H
