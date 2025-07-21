//
// Created by Laurent on 7/21/2025.
//

#ifndef INTERACTABLE_TRANSITION_TO_PRESSED_SYSTEM_H
#define INTERACTABLE_TRANSITION_TO_PRESSED_SYSTEM_H
#include <flecs.h>
#include <raylib.h>

#include "modules/engine/rendering/gui/components.h"

namespace rendering::gui::systems {
    inline void interactable_transition_to_pressed_system(flecs::iter &it, size_t i, const Rectangle &rec) {
        if (!CheckCollisionPointRec(GetMousePosition(), rec)) {
            it.entity(i).add<InteractableElementState>(Normal);
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            it.entity(i).add<InteractableElementState>(Pressed);
        }
    }
}
#endif //INTERACTABLE_TRANSITION_TO_PRESSED_SYSTEM_H
