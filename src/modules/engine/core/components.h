//
// Created by Laurent Voisard on 12/20/2024.
//

#ifndef CORE_COMPONENTS_H
#define CORE_COMPONENTS_H
#include <raylib.h>
#include <string>

namespace core {
    struct Position2D {
        Vector2 value;
    };

    struct Speed {
        float value;
    };

    struct GameSettings {
        std::string windowName;
        int initial_width;
        int initial_height;
        int window_width;
        int window_height;
    };

    struct Tag {
        std::string name;
    };

    struct DestroyAfterTime {
        float time;
    };

    struct DestroyAfterFrame {};

    struct Paused {
        bool paused;
    };

    struct PauseOnEnabled {};
    struct EnabledMenus {int count;};

}

#endif //CORE_COMPONENTS_H
