//
// Created by Laurent on 7/21/2025.
//

#ifndef TOGGLE_ELEMENT_ON_INPUT_SYSTEM_H
#define TOGGLE_ELEMENT_ON_INPUT_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include "modules/engine/input/components.h"
#include "modules/engine/core/components.h"

namespace input::systems {
    inline void toggle_element_on_input_system(flecs::entity e, const KeyBinding &binding, InputToggleEnable) {
        if(IsKeyPressed(binding.key)) {
            flecs::entity pausable = e.parent().parent();
            if(pausable.enabled()) {
                pausable.add<core::Close>();
            } else {
                pausable.add<core::Open>();
            }
        }
    }
}

#endif //TOGGLE_ELEMENT_ON_INPUT_SYSTEM_H
