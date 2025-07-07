//
// Created by laurent on 29/05/25.
//

#ifndef COLLISION_RESOLUTION_SYSTEM_H
#define COLLISION_RESOLUTION_SYSTEM_H

#include <flecs.h>
#include <functional>
#include <raylib.h>
#include <raymath.h>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"
#include "modules/gameplay/components.h"

namespace physics::systems {
    /**
     * Correct the positions of the entity (if they are not static nor "triggers") by the overlap amount
     * @param a entity 1
     * @param a_col entity 1 mutable position
     * @param b entity 2
     * @param b_col entity 2 mutable position
     * @param overlap amount of overlap of the two entities
     */
    inline void correct_positions(flecs::entity &a, const Collider &a_col, CollisionInfo &a_info, flecs::entity &b,
                                  const Collider &b_col, CollisionInfo &b_info, Vector2 overlap) {
        core::Position2D a_pos = a.get_mut<core::Position2D>();
        core::Position2D b_pos = b.get_mut<core::Position2D>();

        float a_move_ratio = 0.5f;
        float b_move_ratio = 0.5f;

        if (a_col.static_body) {
            a_move_ratio = 0;
            b_move_ratio = 1.0f;
        }
        if (b_col.static_body) {
            a_move_ratio = 1.0f;
            b_move_ratio = 0;
        }

        if ((!a_col.correct_position && !a_col.static_body) || (!b_col.correct_position && !b_col.static_body)) {
            a_move_ratio = 0.0f;
            b_move_ratio = 0.0f;
        }

        a_pos.value = a_pos.value - overlap * a_move_ratio * 0.75;
        b_pos.value = b_pos.value + overlap * b_move_ratio * 0.75;
    }

    /**
     * Resolve Circle vs Circle collision
     * @param a circle 1 entity
     * @param base_col circle 1 collider
     * @param b circle 2 entity
     * @param other_base_col circle 2 entity
     * @return if the circles collided
     */
    inline bool handle_circle_circle(flecs::entity &a, const Collider &a_col, CollisionInfo &a_info, flecs::entity &b,
                                     const Collider &b_col, CollisionInfo &b_info) {
        Vector2 a_pos = a.get<core::Position2D>().value;
        Vector2 b_pos = b.get<core::Position2D>().value;

        const CircleCollider col = a.get<CircleCollider>();
        const CircleCollider other_col = b.get<CircleCollider>();

        if (!CheckCollisionCircles(a_pos, col.radius, b_pos, other_col.radius)) {
            return false;
        }

        Vector2 overlap = PhysicsModule::collide_circles(col, a.get<core::Position2D>(), a_info, other_col,
                                                         b.get<core::Position2D>(), b_info);

        correct_positions(a, a_col, a_info, b, b_col, b_info, overlap);

        Vector2 contact_point = a_pos + b_info.normal * col.radius;
        a_info.contact_point = contact_point;
        b_info.contact_point = contact_point;

        return true;
    }

    /**
     * Resolve circle vs rectangle collision
     * @param a circle entity
     * @param a_col circle base collider
     * @param b box entity
     * @param b_col box base collider
     * @return if there was a collision
     */
    inline bool handle_circle_rec_collision(flecs::entity &a, const Collider &a_col, CollisionInfo &a_info,
                                            flecs::entity &b, const Collider &b_col, CollisionInfo &b_info) {
        Vector2 a_pos = a.get<core::Position2D>().value;
        Vector2 b_pos = b.get<core::Position2D>().value;
        const CircleCollider circle_col = a.get<CircleCollider>();
        if (!CheckCollisionCircleRec(
            a_pos, circle_col.radius, {
                b_pos.x + b_col.bounds.x, b_pos.y + b_col.bounds.y, b_col.bounds.width,
                b_col.bounds.height
            })) {
            return false;
        }

        Vector2 overlap = PhysicsModule::collide_circle_rec(
            circle_col, a.get_mut<core::Position2D>(), a_info,
            b_col, b.get_mut<core::Position2D>(), b_info);

        correct_positions(a, a_col, a_info, b, b_col, b_info, overlap);

        Vector2 contact_point = a_pos + b_info.normal * circle_col.radius;
        a_info.contact_point = contact_point;
        b_info.contact_point = contact_point;

        return true;
    }

    /**
     * Resolve rectangle vs rectangle collision
     * @param a box 1 entity
     * @param a_col box 1 collider
     * @param b box 2 entity
     * @param b_col box 2 collider
     * @return if the boxes collided or not
     */
    inline bool handle_boxes_collision(flecs::entity &a, const Collider &a_col, flecs::entity &b,
                                       const Collider &b_col) {
        Vector2 a_pos = a.get<core::Position2D>().value;
        Vector2 b_pos = b.get<core::Position2D>().value;
        if (!CheckCollisionRecs(
            {
                a_pos.x + a_col.bounds.x, a_pos.y + a_col.bounds.y, a_col.bounds.width,
                a_col.bounds.height
            }, {
                b_pos.x + b_col.bounds.x, b_pos.y + b_col.bounds.y, b_col.bounds.width,
                b_col.bounds.height
            })) {
            return false;
        }

        // TODO need to figure overlap for boxes and update the positions properly
        return true;
    }

    using CollisionHandler = std::function<bool(flecs::entity &, const Collider &, CollisionInfo &, flecs::entity &,
                                                const Collider &, CollisionInfo &)>;

    /**
     * Map for collision type handling, we point to the correct position in the array with the Collider type enum
     */
    inline CollisionHandler collision_handler[ColliderType::SIZE][ColliderType::SIZE] = {
        // Circle = 0
        {
            // Circle vs Circle
            [](flecs::entity &a, const Collider &a_col, CollisionInfo &a_info, flecs::entity &b,
               const Collider &b_col, CollisionInfo &b_info) {
                return handle_circle_circle(a, a_col, a_info, b, b_col, b_info);
            },
            // Circle vs Box
            [](flecs::entity &a, const Collider &a_col, CollisionInfo &a_info, flecs::entity &b,
               const Collider &b_col, CollisionInfo &b_info) {
                return handle_circle_rec_collision(a, a_col, a_info, b, b_col, b_info);
            },
        },
        // Box = 1
        {
            // Box vs Circle
            [](flecs::entity &a, const Collider &a_col, CollisionInfo &a_info, flecs::entity &b,
               const Collider &b_col, CollisionInfo &b_info) {
                return handle_circle_rec_collision(b, b_col, b_info, a, a_col, a_info);
            },
            // Box vs Box
            [](flecs::entity &a, const Collider &a_col, CollisionInfo &a_info, flecs::entity &b,
               const Collider &b_col, CollisionInfo &b_info) {
                return handle_boxes_collision(a, a_col, b, b_col);
            },
        }
        // more collisions
    };

    inline void collision_resolution_system(CollisionRecordList &rec) {
        // looping helps with stability
        for (int i = 0; i < 3; i++) {
            for (auto &record: rec.records) {
                flecs::entity a = record.a; // Current entity
                flecs::entity b = record.b; // Colliding entity

                const Collider a_col = a.get<Collider>();
                const Collider b_col = b.get<Collider>();

                // are the entities colliding?
                CollisionInfo a_info;
                CollisionInfo b_info;
                if (!collision_handler[a_col.type][b_col.type](a, a_col, a_info, b, b_col, b_info)) continue;

                // if the entities are of different types (player & enemy) we report it a significant collision
                // enemy vs environment should not be significant. (too many tables)
                // But player vs environment should count (because of projectiles, they might have behaviours specific to obstacles)
                if (i > 0) continue;
                if ((a_col.collision_type & b_col.collision_type) == none &&
                    (a_col.collision_type | b_col.collision_type) != (enemy | environment)) {
                    rec.significant_collisions.push_back({a, b, a_info, b_info});
                }
            }
        }
    }
}
#endif //COLLISION_RESOLUTION_SYSTEM_H
