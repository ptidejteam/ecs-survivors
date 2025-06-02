//
// Created by laurent on 29/05/25.
//

#ifndef COLLISION_RESOLUTION_SYSTEM_H
#define COLLISION_RESOLUTION_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void collision_resolution_system(CollisionRecordList &rec) {
        for (auto &record: rec.records) {
            flecs::entity other = record.b; // Colliding entity
            flecs::entity self = record.a; // Current entity
            Vector2 mypos = self.get<core::Position2D>()->value;
            Vector2 otherPos = other.get<core::Position2D>()->value;
            float combinedRadius = self.get<Collider>()->radius +
                                   other.get<Collider>()->radius;

            // Find the distance and adjust to resolve the overlap
            Vector2 direction = otherPos - mypos;
            Vector2 moveDirection = Vector2Normalize(direction);
            float overlap = combinedRadius - Vector2Length(direction);

            // Move the entities apart by the amount of overlap
            Vector2 move = moveDirection * overlap * 0.5f;

            self.set<Velocity2D>({self.get<Velocity2D>()->value - move});
            other.set<Velocity2D>({other.get<Velocity2D>()->value + move});

            // Resolve by adjusting positions
            self.set<core::Position2D>({mypos - move / 2.f}); // Move the current entity
            other.set<core::Position2D>({otherPos + move / 2.f}); // Move the other entity
        }
        rec.records.clear();
    }
}
#endif //COLLISION_RESOLUTION_SYSTEM_H
