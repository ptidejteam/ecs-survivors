//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef PHYSICS_MODULE_H
#define PHYSICS_MODULE_H


#include <cmath>
#include <vector>

#include "modules/base_module.h"
#include "flecs.h"
#include "modules/engine/core/components.h"
#include "modules/engine/rendering/components.h"

#include <raymath.h>

#include "components.h"


namespace physics {
    constexpr float PHYSICS_TICK_LENGTH = 0.016f;
    static CollisionFilter player_filter = static_cast<CollisionFilter>(enemy | environment);
    static CollisionFilter enemy_filter = static_cast<CollisionFilter>(player | enemy | environment);
    static CollisionFilter environment_filter = static_cast<CollisionFilter>(player | enemy);


    class PhysicsModule : public BaseModule<PhysicsModule> {
        friend class BaseModule<PhysicsModule>;

    public:
        // do not add implementation to the constructor
        PhysicsModule(flecs::world &world): BaseModule(world) {
        };

        static void collide_circles(const CircleCollider *a, core::Position2D *a_pos, bool correct,
                            const CircleCollider *b, core::Position2D *b_pos, bool other_correct) {
            float combinedRadius = a->radius +
                                   b->radius;

            // Find the distance and adjust to resolve the overlap
            Vector2 direction = b_pos->value - a_pos->value;
            Vector2 moveDirection = Vector2Normalize(direction);
            float overlap = combinedRadius - Vector2Length(direction);

            // Move the entities apart by the amount of overlap
            Vector2 move = moveDirection * overlap * 0.5f;

            // Resolve by adjusting positions
            if (correct && other_correct) {
                a_pos->value = a_pos->value - move;
                b_pos->value = b_pos->value + move;
            }
        }

        static void collide_circle_rec(const CircleCollider *a, core::Position2D *a_pos, bool correct,
                            const Collider *b, core::Position2D *b_pos, bool other_correct) {
            float recCenterX = b_pos->value.x + b->bounds.x + b->bounds.width / 2.0f;
            float recCenterY = b_pos->value.y + b->bounds.y + b->bounds.height / 2.0f;

            float halfWidth = b->bounds.width / 2.0f;
            float halfHeight = b->bounds.height / 2.0f;

            float dx = a_pos->value.x - recCenterX;
            float dy = a_pos->value.y - recCenterY;

            float absDx = fabsf(dx);
            float absDy = fabsf(dy);

            if (absDx > (halfWidth + a->radius)) return;
            if (absDy > (halfHeight + a->radius)) return;

            if (absDx <= halfWidth || absDy <= halfHeight) {
                //std::cout << "collided 1" << std::endl;
                // Side collision — resolve with axis-aligned MTV
                float overlapX = (halfWidth + a->radius) - absDx;
                float overlapY = (halfHeight + a->radius) - absDy;

                if (overlapX < overlapY) {
                    a_pos->value = {a_pos->value.x + ((dx < 0) ? -overlapX : overlapX), a_pos->value.y};
                } else {
                    a_pos->value = {a_pos->value.x, a_pos->value.y + ((dy < 0) ? -overlapY : overlapY)};
                }
                return;
            }

            // Corner collision
            float cornerDx = absDx - halfWidth;
            float cornerDy = absDy - halfHeight;

            float cornerDistSq = cornerDx * cornerDx + cornerDy * cornerDy;
            float radius = a->radius;

            if (cornerDistSq < radius * radius) {
                //std::cout << "collided 2" << std::endl;
                float dist = sqrtf(cornerDistSq);

                if (dist == 0.0f) dist = 0.01f; // Avoid divide by zero

                float overlap = radius - dist;
                float nx = cornerDx / dist;
                float ny = cornerDy / dist;

                a_pos->value.x = a_pos->value.x + nx * overlap * ((dx < 0) ? -1.0f : 1.0f);
                a_pos->value.y = a_pos->value.y + ny * overlap * ((dy < 0) ? -1.0f : 1.0f);

                return;
            }
        }

        flecs::entity m_physicsTick;

    private:
        void register_components(flecs::world &world);

        void register_queries(flecs::world &world);

        void register_systems(flecs::world &world);

        void register_pipeline(flecs::world &world);
    };
}

#endif //PHYSICS_MODULE_H
