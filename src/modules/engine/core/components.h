//
// Created by Laurent Voisard on 12/20/2024.
//

#ifndef CORE_COMPONENTS_H
#define CORE_COMPONENTS_H
#include <raylib.h>

namespace core {
    struct Position2D {
        Vector2 value;
    };

    struct Speed {
        float value;
    };

    struct GameSettings {
        std::string windowName;
        int windowWidth;
        int windowHeight;
    };

    struct Damage {
        float value;
    };

    struct Health {
        float max;
        float value;
    };

    struct Player{};
    struct Enemy{};

}

#endif //CORE_COMPONENTS_H
