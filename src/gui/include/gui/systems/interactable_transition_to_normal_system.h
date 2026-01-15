//
// Created by Laurent on 7/21/2025.
//

#ifndef INTERACTABLE_TRANSITION_TO_NORMAL_SYSTEM_H
#define INTERACTABLE_TRANSITION_TO_NORMAL_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

#include "gui/components.h"

namespace gui::systems {
    inline void interactable_transition_to_normal_system(flecs::iter &it, size_t i, const Rectangle &rec) {
        it.entity(i).add<InteractableElementState>(Normal);
    }
}
#endif //INTERACTABLE_TRANSITION_TO_NORMAL_SYSTEM_H
