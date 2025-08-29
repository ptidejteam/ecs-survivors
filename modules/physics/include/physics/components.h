//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_COMPONENTS_H
#define PHYSICS_COMPONENTS_H
#include <vector>
#include <unordered_map>
#include <raylib.h>
#include <flecs.h>
namespace physics {

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

    struct StaticCollider {
    };

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

    struct SignificantCollisionRecord {
        flecs::entity a;
        flecs::entity b;
        CollisionInfo a_info;
        CollisionInfo b_info;
    };

    struct IdPairHash {
        std::size_t operator () (const std::pair<long,long>& h) const {
            auto h1 = std::hash<long>{}(h.first);
            auto h2 = std::hash<long>{}(h.second);
            return h1 ^ (h2 << 1);
        }
    };

    struct CollisionRecordList {
        std::vector<CollisionRecord> records;
        std::vector<SignificantCollisionRecord> significant_collisions;
        std::unordered_map<std::pair<long,long>, CollisionInfo, IdPairHash> collisions_info;
    };

    struct SpatialHashingGrid {
        int cell_size;
        Vector2 offset;
        std::unordered_map<std::pair<long,long>, flecs::entity, IdPairHash> cells;
    };

    struct GridCell {
        int x;
        int y;
        std::vector<flecs::entity> entities;
    };

    struct ContainedIn {};


}

#endif //PHYSICS_COMPONENTS_H
