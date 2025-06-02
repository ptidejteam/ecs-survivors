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

        flecs::entity m_physicsTick;
    private:



        void register_components(flecs::world &world);

        void register_queries(flecs::world &world);

        void register_systems(flecs::world &world);

        void register_pipeline(flecs::world &world);

    };
}

#endif //PHYSICS_MODULE_H
