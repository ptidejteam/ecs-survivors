//
// Created by Laurent on 7/21/2025.
//

#ifndef INVOKE_BUTTON_CALLBACK_SYSTEM_H
#define INVOKE_BUTTON_CALLBACK_SYSTEM_H

#include <flecs.h>
#include "gui/components.h"

namespace rendering::gui::systems {
    inline void invoke_button_callback_system(flecs::entity e, const ButtonCallback &callback) {
        callback.callback();
    }
}
#endif //INVOKE_BUTTON_CALLBACK_SYSTEM_H
