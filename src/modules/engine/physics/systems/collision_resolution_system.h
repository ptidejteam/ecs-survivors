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
    inline bool handle_circle_circle(flecs::entity &a, const Collider *base_col, flecs::entity &b,
                                     const Collider *other_base_col) {
        Vector2 mypos = a.get<core::Position2D>()->value;
        Vector2 otherPos = b.get<core::Position2D>()->value;

        const CircleCollider *col = a.get<CircleCollider>();
        const CircleCollider *other_col = b.get<CircleCollider>();

        if (!CheckCollisionCircles(mypos, col->radius, otherPos, other_col->radius)) return false;
        if (!base_col->correct_position || !other_base_col->correct_position) return true;
        PhysicsModule::collide_circles(
            col, a.get_mut<core::Position2D>(), base_col->correct_position,
            other_col, b.get_mut<core::Position2D>(), other_base_col->correct_position);
        return true;
    }

    inline bool handle_circle_rec_collision(flecs::entity &circle, const Collider *base_col, flecs::entity &b,
                                            const Collider *other_base_col) {
        Vector2 mypos = circle.get<core::Position2D>()->value;
        Vector2 otherPos = b.get<core::Position2D>()->value;
        const CircleCollider *circle_col = circle.get<CircleCollider>();
        if (!CheckCollisionCircleRec(mypos, circle_col->radius, {
                                         otherPos.x + other_base_col->bounds.x,
                                         otherPos.y + other_base_col->bounds.y, other_base_col->bounds.width,
                                         other_base_col->bounds.height
                                     })) {
            return false;
        }

        PhysicsModule::collide_circle_rec(
            circle_col, circle.get_mut<core::Position2D>(), base_col->correct_position,
            other_base_col, b.get_mut<core::Position2D>(), other_base_col->correct_position);
        return true;
    }

    inline void collision_resolution_system(CollisionRecordList &rec) {
        //std::cout << rec.records.size() << std::endl;
        for (auto &record: rec.records) {
            flecs::entity other = record.b; // Colliding entity
            flecs::entity self = record.a; // Current entity

            const Collider *base_col = self.get<Collider>();
            const Collider *other_base_col = other.get<Collider>();


            bool collided = false;

            if (self.has<CircleCollider>() && other.has<CircleCollider>()) {
                collided = handle_circle_circle(self, base_col, other, base_col);
            } else if (self.has<BoxCollider>() && other.has<CircleCollider>()) {
                collided = handle_circle_rec_collision(other, other_base_col, self, base_col);
            } else if (self.has<CircleCollider>() && other.has<BoxCollider>()) {
                collided = handle_circle_rec_collision(self, base_col, other, other_base_col);
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
