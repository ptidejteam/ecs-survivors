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
#include "modules/gameplay/components.h"

namespace physics::systems {
    inline void collision_resolution_system(CollisionRecordList &rec) {
        //std::cout << rec.records.size() << std::endl;
        for (auto &record: rec.records) {
            flecs::entity other = record.b; // Colliding entity
            flecs::entity self = record.a; // Current entity
            Vector2 mypos = self.get<core::Position2D>()->value;
            Vector2 otherPos = other.get<core::Position2D>()->value;

            const Collider *base_col = self.get<Collider>();
            const Collider *other_base_col = other.get<Collider>();

            bool collided = false;

            if (self.has<CircleCollider>() && other.has<CircleCollider>()) {
                const CircleCollider *col = self.get<CircleCollider>();
                const CircleCollider *other_col = other.get<CircleCollider>();
                if (!CheckCollisionCircles(mypos, col->radius, otherPos, other_col->radius)) continue;
                PhysicsModule::collide_circles(
                    col, self.get_mut<core::Position2D>(), base_col->correct_position,
                    other_col, other.get_mut<core::Position2D>(), other_base_col->correct_position);

                collided = true;
            } else if (self.has<BoxCollider>() && other.has<CircleCollider>()) {
                const CircleCollider *circle_col = other.get<CircleCollider>();
                if (!CheckCollisionCircleRec(otherPos, circle_col->radius, {
                                                 mypos.x + base_col->bounds.x, mypos.y + base_col->bounds.y,
                                                 base_col->bounds.width, base_col->bounds.height
                                             })) {
                    continue;
                }
                PhysicsModule::collide_circle_rec(
                    circle_col, other.get_mut<core::Position2D>(), other_base_col->correct_position,
                    base_col, self.get_mut<core::Position2D>(), base_col->correct_position);

                collided = true;
            } else if (self.has<CircleCollider>() && other.has<BoxCollider>()) {
                const CircleCollider *circle_col = self.get<CircleCollider>();
                if (!CheckCollisionCircleRec(mypos, circle_col->radius, {
                                                 otherPos.x + other_base_col->bounds.x,
                                                 otherPos.y + other_base_col->bounds.y, other_base_col->bounds.width,
                                                 other_base_col->bounds.height
                                             })) {
                    continue;
                }

                PhysicsModule::collide_circle_rec(
                    circle_col, self.get_mut<core::Position2D>(), base_col->correct_position,
                    other_base_col, other.get_mut<core::Position2D>(), other_base_col->correct_position);

                collided = true;
            } else if (self.has<BoxCollider>() && other.has<BoxCollider>()) {
                collided = true;
            }

            if (!collided) continue;
            if ((base_col->collision_type & other_base_col->collision_type) == none) {
                rec.significant_collisions.push_back({self, other});
            }
        }
        rec.records.clear();
    }
}
#endif //COLLISION_RESOLUTION_SYSTEM_H
