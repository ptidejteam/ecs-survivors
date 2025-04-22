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
        int initialWidth;
        int initialHeight;
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

    struct Attack {
        std::string attack_prefab_name;
        std::string target_tag;
    };

    struct Tag {
        std::string name;
    };

    struct DestroyAfterTime {
        float time;
    };

    struct DestroyAfterFrame {};

}

#endif //CORE_COMPONENTS_H
