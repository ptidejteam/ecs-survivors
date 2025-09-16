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
    struct PausesRequested {int count;};

    struct Close {};
    struct Open {};

    struct ExitRequested {};
    struct ExitConfirmed {};

}

#endif //CORE_COMPONENTS_H
