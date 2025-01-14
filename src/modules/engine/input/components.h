//
// Created by Laurent Voisard on 12/29/2024.
//


#ifndef INPUT_COMPONENTS_H
#define INPUT_COMPONENTS_H

#include <optional>

namespace input {
    struct InputHorizontal {
      float value;
    };

    struct InputVertical {
        float value;
    };

    struct KeyBinding {
        int key = -1;
        float value = 1;
    };

    struct BindingSlot1 {};
    struct BindingSlot2 {};
    struct BindingSlot3 {};
    struct BindingSlot4 {};
}

#endif //INPUT_COMPONENTS_H
