//
// Created by laurent on 29/05/25.
//

#ifndef COLLISION_DETECTION_SYSTEM_H
#define COLLISION_DETECTION_SYSTEM_H

#include <flecs.h>
#include <mutex>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

#include "modules/engine/physics/queries.h"

namespace physics::systems {
    std::mutex list_mutex;

    inline void collision_detection_system(flecs::iter &self_it, size_t self_id, CollisionRecordList &list,
                                           const core::Position2D &pos,
                                           const Collider &collider) {
        std::vector<CollisionRecord> collisions;
        flecs::world stage_world = self_it.world();

        // Build a staged query, and filter
        auto visible_query = stage_world.query_builder<const core::Position2D, const Collider>()
                .with<rendering::Visible>().filter();

        visible_query.each(
            [&](flecs::iter &other_it, size_t other_id, const core::Position2D &other_pos,
                const Collider &other_collider) {
                if (self_id > other_id) return;
                if ((collider.collision_filter & other_collider.collision_type) == none) return;

                float rad = collider.radius + other_collider.radius;
                if (Vector2DistanceSqr(pos.value, other_pos.value) < rad * rad) {
                    flecs::entity self = self_it.entity(self_id);
                    flecs::entity other = other_it.entity(other_id);

                    if (collider.correct_position && other_collider.correct_position)
                        collisions.push_back({self, other});

                    if ((collider.collision_type & other_collider.collision_type) == none) {
                        self.add<CollidedWith>(other);
                        other.add<CollidedWith>(self);
                    }
                }
            });


        list_mutex.lock();
        list.records.insert(list.records.end(), collisions.begin(), collisions.end());
        list_mutex.unlock();
        collisions.clear();
    }
}
#endif //COLLISION_DETECTION_SYSTEM_H
