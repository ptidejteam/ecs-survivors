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
    static CollisionFilter player_filter = static_cast<CollisionFilter>(player | enemy);
    static CollisionFilter enemy_filter = static_cast<CollisionFilter>(player | enemy);


    class PhysicsModule : public BaseModule<PhysicsModule> {
        friend class BaseModule<PhysicsModule>;

    public:
        // do not add implementation to the constructor
        PhysicsModule(flecs::world &world): BaseModule(world) {};

        inline static flecs::entity m_physicsTick;
    private:



        void register_components(flecs::world &world);

        //void register_pipeline(flecs::world& world) = delete;
        void register_systems(flecs::world &world);


        static bool collide(flecs::entity &self, flecs::entity &other) {
            Vector2 mypos = self.get<core::Position2D>()->value;
            Vector2 otherPos = other.get<core::Position2D>()->value;
            float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
                                   radius;

            // Find the distance and adjust to resolve the overlap
            Vector2 direction = otherPos - mypos;

            float distanceSqr = Vector2LengthSqr(direction);

            return distanceSqr < combinedRadius * combinedRadius;
        }

    };
}

#endif //PHYSICS_MODULE_H
