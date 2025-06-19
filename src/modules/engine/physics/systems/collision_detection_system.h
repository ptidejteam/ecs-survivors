//
// Created by laurent on 29/05/25.
//

#ifndef COLLISION_DETECTION_SYSTEM_H
#define COLLISION_DETECTION_SYSTEM_H

#include <flecs.h>
#include <mutex>

#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace physics::systems {
    static std::mutex list_mutex;

    inline void collision_detection_system(flecs::iter &self_it, size_t self_id, CollisionRecordList &list,
                                           const core::Position2D &pos,
                                           const Collider &collider) {
        std::vector<CollisionRecord> collisions;
        std::vector<CollisionRecord> events;
        flecs::world stage_world = self_it.world();

        // Build a staged query, and filter
        auto visible_query = stage_world.query_builder<const core::Position2D, const Collider>()
                .with<rendering::Visible>().build();
        flecs::entity self = self_it.entity(self_id);

        visible_query.each(
            [&](flecs::entity other, const core::Position2D &other_pos,
                const Collider &other_collider) {
                if (other.id() >= self.id()) return;

                if ((collider.collision_filter & other_collider.collision_type) == none) return;

                Rectangle self_rec = {
                    pos.value.x + collider.bounds.x, pos.value.y + collider.bounds.y, collider.bounds.width,
                    collider.bounds.height
                };
                Rectangle other_rec = {
                    other_pos.value.x + other_collider.bounds.x, other_pos.value.y + other_collider.bounds.y,
                    other_collider.bounds.width, other_collider.bounds.height
                };

                if (CheckCollisionRecs(self_rec, other_rec)) {
                    collisions.push_back({self, other});
                }
            });


        // not ideal, there is a bit of loss of time because of the lock
        list_mutex.lock();
        list.records.insert(list.records.end(), collisions.begin(), collisions.end());
        list_mutex.unlock();
    }
}
#endif //COLLISION_DETECTION_SYSTEM_H
