//
// Created by laurent on 9/11/25.
//

#ifndef GUI_UPDATE_INTERACTABLE_TEXUTRED_ELEMENT_SYSTEM_H
#define GUI_UPDATE_INTERACTABLE_TEXUTRED_ELEMENT_SYSTEM_H
#include <flecs.h>
#include "gui/components.h"

namespace gui::systems {
    inline void update_interactable_textured_element_system(flecs::entity e, TexturedElement &tex,
                                                            const InteractableElement inter) {

        if (e.has<InteractableElementState>(Hovered)) {
            tex.tint = inter.hovered_tint;
        } else if (e.has<InteractableElementState>(Pressed)) {
            tex.tint = inter.selected_tint;
        } else {
            tex.tint = inter.normal_tint;
        }
    }

} // namespace gui

#endif // GUI_UPDATE_INTERACTABLE_TEXUTRED_ELEMENT_SYSTEM_H
