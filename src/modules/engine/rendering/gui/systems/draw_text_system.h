//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_TEXT_SYSTEM_H
#define DRAW_TEXT_SYSTEM_H

#include <raylib.h>
#include "modules/engine/rendering/gui/components.h"
#include "modules/engine/rendering/gui/gui_module.h"

namespace rendering::gui::systems {
    inline void draw_text_system(flecs::entity e, const Text &text, const Rectangle &rect, const InteractableElement *inter, const FontAtlas &fonts) {
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

        if (!inter) {
            DrawTextEx(fonts.fonts.at(text.font_size), text.text.c_str(), pos, text.font_size, 0, text.color);
            return;
        }

        Color text_col = text.color;
        if (!e.has<InteractableElementState>(Normal)) {
            Color bkg;
            if (e.has<InteractableElementState>(Hovered)) {
                bkg = inter->hovered_tint;
            } else {
                bkg = inter->selected_tint;
            }

            // https://gamedev.stackexchange.com/questions/38536/given-a-rgb-color-x-how-to-find-the-most-contrasting-color-y
            // Determine if the background color is better for black or white and adjust text color
            const float gamma = 2.2;
            float L = 0.2126 * pow( bkg.r, gamma )
                    + 0.7152 * pow( bkg.g, gamma )
                    + 0.0722 * pow( bkg.b, gamma );

            bool use_black = ( L > pow( 0.5, gamma ) );

            text_col = use_black ? DARKGRAY : LIGHTGRAY;
        }

        DrawTextEx(fonts.fonts.at(text.font_size), text.text.c_str(), pos, text.font_size, 0, text_col);

    }
}
#endif //DRAW_TEXT_SYSTEM_H
