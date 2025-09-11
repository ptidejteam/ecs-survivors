//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_BUTTON_SYSTEM_H
#define DRAW_BUTTON_SYSTEM_H

#include <raylib.h>
#include <flecs.h>
#include "gui/components.h"


namespace gui::systems {
    inline void draw_textured_element_system(TexturedElement& tex, const Rectangle &rect) {
        DrawTextureNPatch(tex.texture, tex.info, rect, {0,0}, 0.0f, tex.tint);
    }
}
#endif //DRAW_BUTTON_SYSTEM_H
