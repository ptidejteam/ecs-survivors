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

#include "box2d/base.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include "box2d/math_functions.h"

enum PHYSICS_COLLISION_STRATEGY {
    COLLISION_RELATIONSHIP,
    COLLISION_ENTITY,
    SPATIAL_HASH_REBUILDING,
    SPATIAL_HASH_UPDATING,
    BOX2D
};

namespace physics {
    constexpr float PHYSICS_TICK_LENGTH = 0.016f;

    class PhysicsModule : public BaseModule<PhysicsModule> {
        friend class BaseModule<PhysicsModule>;

    public:
        // do not add implementation to the constructor
        PhysicsModule(flecs::world &world): BaseModule(world) {
            m_spatial_hash = new SpatialHashRebuilding();
            //change_collision_strategy(BOX2D);
        };

        inline static flecs::entity s1, s2, s3, s4, s5, s6, add_entity_to_physics, remove_entity_to_physics, s8, s9, s10
                , s_update_position, s_update_box2d_velocity, s_box2d_step, s_position_from_box2d;


        static void change_collision_strategy(PHYSICS_COLLISION_STRATEGY type) {
            collision_strategy = type;
            s1.disable();
            s2.disable();
            s3.disable();
            s4.disable();
            s5.disable();
            s6.disable();
            remove_entity_to_physics.disable();
            add_entity_to_physics.disable();
            s8.disable();
            s9.disable();
            s_update_position.enable();
            s_box2d_step.disable();
            s_position_from_box2d.disable();

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
                    remove_entity_to_physics.enable();
                    s8.enable();
                    s9.enable();
                    break;
                case SPATIAL_HASH_UPDATING:
                    m_spatial_hash = new SpatialHashUpdating();
                    s6.enable();
                    remove_entity_to_physics.enable();
                    add_entity_to_physics.enable();
                    s8.enable();
                    s9.enable();
                //s10.enable();
                    break;
                case BOX2D:
                    worldDef = b2DefaultWorldDef();
                    worldDef.workerCount = 8;
                    worldDef.gravity = b2Vec2(0.0f, 0.0f);
                    worldId = b2CreateWorld(&worldDef);
                    remove_entity_to_physics.enable();
                    add_entity_to_physics.enable();
                    s_update_position.disable();
                    s_update_box2d_velocity.enable();
                    s_box2d_step.enable();
                    s_position_from_box2d.enable();
            }
        }

    private:
        inline static BaseSpatialHash *m_spatial_hash = nullptr;
        inline static b2WorldDef worldDef;
        inline static b2WorldId worldId;
        inline static PHYSICS_COLLISION_STRATEGY collision_strategy;

        flecs::entity m_physicsTick;


        void register_components(flecs::world &world);

        //void register_pipeline(flecs::world& world) = delete;
        void register_systems(flecs::world &world);

        // call the spatial hash implementation
        void add_entity_to_grid(flecs::entity &entity) {
            if (collision_strategy == SPATIAL_HASH_REBUILDING || collision_strategy == SPATIAL_HASH_UPDATING) {
                m_spatial_hash->add_entity(entity);
            } else if (collision_strategy == BOX2D) {
                create_body_for_entity(entity);
            }
        }

        // call the spatial hash implementation
        void remove_entity_from_grid(flecs::entity &entity) {
            if (collision_strategy == SPATIAL_HASH_REBUILDING || collision_strategy == SPATIAL_HASH_UPDATING) {
                m_spatial_hash->remove_entity(entity);
            }
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

        void create_body_for_entity(flecs::entity &e) {
            if (e.has<Box2DID>()) return;
            auto pos = e.get<core::Position2D>()->value;
            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = b2_dynamicBody;
            bodyDef.position = b2Vec2(pos.x, pos.y);
            b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
            b2ShapeDef shapeDef = b2DefaultShapeDef();
            shapeDef.density = 1.0f;
            shapeDef.friction = 0.3f;
            b2Circle circle;
            circle.center = {0,0};
            circle.radius = 8.0f;
            b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);
            e.set<Box2DID>({bodyId, shapeId});
        }

        // Neighboring cells (including the current cell itself)


        // A custom hash function for pair<int, int> key in the unordered_map
    };
}

#endif //PHYSICS_MODULE_H
