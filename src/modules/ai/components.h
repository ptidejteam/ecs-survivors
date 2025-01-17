//
// Created by Laurent Voisard on 1/10/2025.
//

#ifndef AI_COMPONENTS_H
#define AI_COMPONENTS_H

#include <string>

namespace ai {

    struct Target {
        std::string name;
    };

    struct FollowTarget {};

    struct StoppingDistance {
        float value;
    };
}

#endif //AI_COMPONENTS_H
