//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H
#include <raylib.h>
#include <box2d/id.h>

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

    struct Collider {
        float radius;
    };

    struct Box2DID {
        b2BodyId bodyId;
        b2ShapeId shapeId;
    };

    struct CollidedWith {};

    struct CollisionRecord {
        flecs::entity a;
        flecs::entity b;
    };

    struct CollisionDetectionPhaseCompleted {
        long time;
    };

    struct CollisionResolutionPhaseCompleted {
        long time;
    };

    struct ContainedBy {};
    struct Contains {};
    struct Cell {
        int x;
        int y;
    };

    struct CellHash {
        std::pair<int, int> cell;
    };
}

#endif //PHYSICS_COMPONENTS_H
