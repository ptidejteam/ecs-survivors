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
#include <algorithm>

#include "components.h"
#include <utility>

#include "base_spatial_hash.h"
#include "spatial_hash_rebuilding.h"
#include "spatial_hash_updating.h"

enum PHYSICS_COLLISION_STRATEGY {
    COLLISION_RELATIONSHIP,
    COLLISION_ENTITY,
    SPATIAL_HASH_REBUILDING,
    SPATIAL_HASH_UPDATING
};

namespace physics {
    constexpr float PHYSICS_TICK_LENGTH = 0.016f;

    class PhysicsModule : public BaseModule<PhysicsModule> {
        friend class BaseModule;

    public:
        // do not add implementation to the constructor
        PhysicsModule(flecs::world &world): BaseModule(world) {
            m_spatial_hash = new SpatialHashRebuilding();
        };

        inline static flecs::entity s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;


        static void change_collision_strategy(PHYSICS_COLLISION_STRATEGY type) {

            s1.disable();
            s2.disable();
            s3.disable();
            s4.disable();
            s5.disable();
            s6.disable();
            s7.disable();
            s8.disable();
            s9.disable();
            //s10.disable();
            delete m_spatial_hash;
            switch (type) {
                case COLLISION_RELATIONSHIP:
                    s1.enable();
                    s2.enable();
                    break;
                case COLLISION_ENTITY:
                    s3.enable();
                    s4.enable();
                    s5.enable();
                    break;
                case SPATIAL_HASH_REBUILDING:
                    m_spatial_hash = new SpatialHashRebuilding();
                    s6.enable();
                    s7.enable();
                    s8.enable();
                    s9.enable();
                    //s10.enable();
                    break;
                case SPATIAL_HASH_UPDATING:
                    m_spatial_hash = new SpatialHashUpdating();
                    s6.enable();
                    s7.enable();
                    s8.enable();
                    s9.enable();
                    //s10.enable();
                    break;
            }
        }

    private:
        inline static BaseSpatialHash *m_spatial_hash = nullptr;
        flecs::entity m_physicsTick;


        void register_components(flecs::world &world);

        //void register_pipeline(flecs::world& world) = delete;
        void register_systems(flecs::world &world);

        // call the spatial hash implementation
        void remove_entity_from_grid(flecs::entity &entity) {
            m_spatial_hash->remove_entity(entity);
        }

        // call the spatial hash implementation
        void add_entity_to_grid(flecs::entity &entity) {
            m_spatial_hash->add_entity(entity);
        }

        // Update the position of an entity and rehash if necessary
        void update_entity_position(flecs::entity &entity) {
            m_spatial_hash->update_entity_position(entity);
        }

        void detect_collisions(Cell cell) {
            m_spatial_hash->detect_collisions(cell);
        }

        bool collide(flecs::entity &self, flecs::entity &other) {
            Vector2 mypos = self.get<core::Position2D>()->value;
            Vector2 otherPos = other.get<core::Position2D>()->value;
            float combinedRadius = self.get<rendering::Circle>()->radius + other.get<rendering::Circle>()->
                                   radius;

            // Find the distance and adjust to resolve the overlap
            Vector2 direction = otherPos - mypos;

            float distanceSqr = Vector2LengthSqr(direction);

            return distanceSqr < combinedRadius * combinedRadius;
        }

        void resolve_collisions() {
            m_spatial_hash->resolve_collisions();
        }

        // Neighboring cells (including the current cell itself)


        // A custom hash function for pair<int, int> key in the unordered_map
    };
}

#endif //PHYSICS_MODULE_H
