//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H
#include <raylib.h>

namespace physics {
    struct Velocity2D {
      Vector2 value;
    };
    struct DesiredVelocity2D {
        Vector2 value;
    };
    struct AccelerationSpeed {
        float value;
    };
}

#endif //PHYSICS_COMPONENTS_H
