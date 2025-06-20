//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H
#include <raylib.h>
#include <vector>

namespace physics {
    struct CircleCollider;
    struct BoxCollider;

    enum CollisionFilter {
        none = 0x00,
        player = 0x01,
        enemy = 0x02,
        environment = 0x04,
    };

    enum ColliderType {
        Circle = 0,
        Box = 1,
        SIZE = 2
        // ... more
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
        bool correct_position;
        bool static_body;
        Rectangle bounds;
        CollisionFilter collision_type;
        CollisionFilter collision_filter;
        ColliderType type;
    };

    struct StaticCollider {};

    struct CircleCollider {
        float radius;
    };

    struct BoxCollider {
    };

    struct CollidedWith {
    };

    struct CollisionInfo {
        Vector2 normal;
        Vector2 contact_point;
    };

    struct CollisionRecord {
        flecs::entity a;
        flecs::entity b;
    };


    struct CollisionRecordList {
        std::vector<CollisionRecord> records;
        std::vector<CollisionRecord> significant_collisions;
        std::unordered_map<long, CollisionInfo> collisions_info;
    };
}

#endif //PHYSICS_COMPONENTS_H
