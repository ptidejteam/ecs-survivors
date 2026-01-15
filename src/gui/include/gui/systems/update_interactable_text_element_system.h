//
// Created by laurent on 9/11/25.
//

#ifndef GUI_SURVIVORS_UPDATE_INTERACTABLE_TEXT_ELEMENT_SYSTEM_H
#define GUI_SURVIVORS_UPDATE_INTERACTABLE_TEXT_ELEMENT_SYSTEM_H
#include "gui/components.h"

namespace gui::systems {
    inline void update_interactable_text_element_system(flecs::entity e, Text &tex, const InteractableElement inter) {

        if (e.has<InteractableElementState>(Hovered)) {
            tex.color = inter.hovered_tint;
        } else if (e.has<InteractableElementState>(Pressed)) {
            tex.color = inter.selected_tint;
        } else {
            tex.color = inter.normal_tint;
        }
    }

} // namespace gui::systems

#endif // GUI_SURVIVORS_UPDATE_INTERACTABLE_TEXT_ELEMENT_SYSTEM_H
