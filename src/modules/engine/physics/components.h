//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H
#include <raylib.h>
#include <vector>

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
        bool correct_position;
        CollisionFilter collision_type;
        CollisionFilter collision_filter;
    };

    struct CollidedWith {};

    struct CollisionRecord {
        flecs::entity a;
        flecs::entity b;
    };

    struct CollisionRecordList {
        std::vector<CollisionRecord> records;
        std::vector<CollisionRecord> significant_collisions;
    };
}

#endif //PHYSICS_COMPONENTS_H
