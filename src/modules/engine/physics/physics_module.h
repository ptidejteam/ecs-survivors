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

    inline flecs::system m_collision_detection_spatial_hashing_system;
    inline flecs::system m_collision_detection_spatial_ecs;
    inline flecs::system m_collision_detection_naive_system;

    class PhysicsModule : public BaseModule<PhysicsModule> {
        friend class BaseModule<PhysicsModule>;

    public:
        // do not add implementation to the constructor
        PhysicsModule(flecs::world &world): BaseModule(world) {
        };

        static Vector2 collide_circles(const CircleCollider &a, const core::Position2D &a_pos, CollisionInfo& a_info,
                                       const CircleCollider &b, const core::Position2D &b_pos, CollisionInfo& b_info) {
            float combinedRadius = a.radius + b.radius;

            // Find the distance and adjust to resolve the overlap
            Vector2 direction = b_pos.value - a_pos.value;
            Vector2 moveDirection = Vector2Normalize(direction);
            float overlap = combinedRadius - Vector2Length(direction);

            a_info.normal = Vector2Negate(moveDirection);
            b_info.normal = moveDirection;

            return moveDirection * overlap;
        }

        static Vector2 collide_circle_rec(const CircleCollider &a, core::Position2D &a_pos, CollisionInfo& a_info,
                                          const Collider &b, core::Position2D &b_pos, CollisionInfo& b_info) {
            float recCenterX = b_pos.value.x + b.bounds.x + b.bounds.width / 2.0f;
            float recCenterY = b_pos.value.y + b.bounds.y + b.bounds.height / 2.0f;

            float halfWidth = b.bounds.width / 2.0f;
            float halfHeight = b.bounds.height / 2.0f;

            float dx = a_pos.value.x - recCenterX;
            float dy = a_pos.value.y - recCenterY;

            float absDx = fabsf(dx);
            float absDy = fabsf(dy);

            Vector2 overlap = {0, 0};

            if (absDx > (halfWidth + a.radius)) return overlap;
            if (absDy > (halfHeight + a.radius)) return overlap;

            if (absDx <= halfWidth || absDy <= halfHeight) {
                // Side collision — resolve with axis-aligned MTV
                float overlapX = (halfWidth + a.radius) - absDx;
                float overlapY = (halfHeight + a.radius) - absDy;


                if (overlapX < overlapY) {
                    overlap.x = dx < 0 ? overlapX : -overlapX;
                } else {
                    overlap.y = dy < 0 ? overlapY : -overlapY;
                }
                a_info.normal = Vector2Normalize(Vector2Negate(overlap));
                b_info.normal = Vector2Normalize(overlap);
                return overlap;
            }

            // Corner collision
            float cornerDx = absDx - halfWidth;
            float cornerDy = absDy - halfHeight;

            float cornerDistSq = cornerDx * cornerDx + cornerDy * cornerDy;
            float radius = a.radius;

            if (cornerDistSq < radius * radius) {
                //std::cout << "collided 2" << std::endl;
                float dist = sqrtf(cornerDistSq);

                if (dist == 0.0f) dist = 0.01f; // Avoid divide by zero

                float overlap_length = radius - dist;
                float nx = cornerDx / dist;
                float ny = cornerDy / dist;

                overlap = {
                    nx * overlap_length * ((dx < 0) ? 1.0f : -1.0f),
                    ny * overlap_length * ((dy < 0) ? 1.0f : -1.0f)
                };

                a_info.normal = Vector2Normalize(Vector2Negate(overlap));
                b_info.normal = Vector2Normalize(overlap);
            }

            return overlap;
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
