//
// Created by Laurent Voisard on 12/29/2024.
//


#ifndef INPUT_COMPONENTS_H
#define INPUT_COMPONENTS_H

namespace input {
    struct InputHorizontal {
      float value;
    };

    struct InputVertical {
        float value;
    };

    struct KeyBinding {
        int key;
        float value;
    };
}

#endif //INPUT_COMPONENTS_H
