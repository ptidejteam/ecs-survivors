//
// Created by laure on 1/10/2025.
//

#ifndef AI_COMPONENTS_H
#define AI_COMPONENTS_H

#include <string>

namespace ai {

    struct FollowTarget {};

    struct Target {
        std::string name;
    };

    struct StoppingDistance {
        float value;
    };
}

#endif //AI_COMPONENTS_H
