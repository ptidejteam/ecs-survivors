//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H
#include <raylib.h>
#include <unordered_set>

namespace physics {
    enum CollisionFilter {
        none = 0x00,
        player = 0x01,
        enemy = 0x02,
        more = 0x04,
    };

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
        CollisionFilter collision_type;
        CollisionFilter collision_filter;
    };

    struct OnCollisionDetected {
        flecs::entity other;
    };

    struct CollidedWith {};
    struct Test{};

    struct CollisionRecord {
        flecs::entity a;
        flecs::entity b;
    };

    struct CollisionRecordList {
        std::vector<CollisionRecord> records;
    };
}

#endif //PHYSICS_COMPONENTS_H
