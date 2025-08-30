//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_BUTTON_SYSTEM_H
#define DRAW_BUTTON_SYSTEM_H

#include <raylib.h>
#include <flecs.h>
#include "gui/components.h"


namespace gui::systems {
    inline void draw_interactable_textured_element_system(flecs::entity e, const TexturedElement& tex, const InteractableElement& inter,
                                   const Rectangle &rect) {
        Color tint;

        if (e.has<InteractableElementState>(Hovered)) {
            tint = inter.hovered_tint;
        } else if (e.has<InteractableElementState>(Pressed)) {
            tint = inter.selected_tint;
        } else {
            tint = inter.normal_tint;
        }

        DrawTextureNPatch(tex.texture, tex.info, rect, {0,0}, 0.0f, tint);
    }
}
#endif //DRAW_BUTTON_SYSTEM_H
