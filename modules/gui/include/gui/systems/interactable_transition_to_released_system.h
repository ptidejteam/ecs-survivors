//
// Created by Laurent on 7/21/2025.
//

#ifndef INTERACTABLE_TRANSITION_TO_RELEASED_SYSTEM_H
#define INTERACTABLE_TRANSITION_TO_RELEASED_SYSTEM_H
#include <flecs.h>
#include <raylib.h>

#include "gui/components.h"

namespace gui::systems {
    inline void interactable_transition_to_released_system(flecs::iter &it, size_t i, const Rectangle &rec) {
        if (!CheckCollisionPointRec(GetMousePosition(), rec)) {
            it.entity(i).add<InteractableElementState>(Normal);
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            it.entity(i).add<InteractableElementState>(Released);
        }
    }
}
#endif //INTERACTABLE_TRANSITION_TO_RELEASED_SYSTEM_H
