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


    inline void collision_detection_system(flecs::iter &self_it, size_t self_id,
                                           std::vector<CollisionRecord> &thread_local_records,
                                           std::vector<CollisionRecord> &thread_local_events,
                                           core::Position2D &pos,
                                           Collider &collider) {
        //std::vector<CollisionRecord> collisions;
        //std::vector<CollisionRecord> events;
        flecs::world stage_world = self_it.world();

        // Build a staged query, and filter
        auto visible_query = stage_world.query_builder<core::Position2D, Collider>()
                .with<rendering::Visible>().filter();

        visible_query.each(
            [&](flecs::iter &other_it, size_t other_id, core::Position2D &other_pos,
                Collider &other_collider) {
                if (self_id > other_id) return;
                if ((collider.collision_filter & other_collider.collision_type) == none) return;

                float rad = collider.radius + other_collider.radius;
                if (Vector2DistanceSqr(pos.value, other_pos.value) < rad * rad) {
                    flecs::entity self = self_it.entity(self_id);
                    flecs::entity other = other_it.entity(other_id);

                    if (collider.correct_position && other_collider.correct_position) {

                        // Find the distance and adjust to resolve the overlap
                        Vector2 direction = other_pos.value - pos.value;
                        Vector2 moveDirection = Vector2Normalize(direction);
                        float overlap = rad - Vector2Length(direction);

                        // Move the entities apart by the amount of overlap
                        Vector2 move = moveDirection * overlap * 0.5f;

                        //self.set<Velocity2D>({self.get<Velocity2D>()->value - move});
                        //other.set<Velocity2D>({other.get<Velocity2D>()->value + move});

                        // Resolve by adjusting positions
                        pos.value = pos.value - move / 2.f; // Move the current entity
                        other_pos.value = other_pos.value + move / 2.f; // Move the other entity
                    }
                    //    thread_local_records.push_back({self, other});

                    if ((collider.collision_type & other_collider.collision_type) == none) {
                        //thread_local_events.push_back({self,other});
                        self.add<CollidedWith>(other);
                        other.add<CollidedWith>(self);
                    }
                }
            });
    }

    inline void collision_detection_system_threadsafe(
        flecs::iter &it, size_t self_id,
        CollisionRecordList &list,
        core::Position2D &pos,
        Collider &collider) {
        // Thread-local buffer
        std::vector<CollisionRecord> thread_local_records;
        std::vector<CollisionRecord> thread_local_events;

        // Run actual logic
        collision_detection_system(it, self_id, thread_local_records, thread_local_events, pos, collider);

        // Merge thread-local buffer into shared record list
        // list_mutex.lock();
        // list.records.insert(
        //     list.records.end(),
        //     thread_local_records.begin(),
        //     thread_local_records.end()
        // );
        //
        // list.significant_collisions.insert(
        //     list.significant_collisions.end(),
        //     thread_local_events.begin(),
        //     thread_local_events.end()
        // );
        // list_mutex.unlock();
    }
}
#endif //COLLISION_DETECTION_SYSTEM_H
