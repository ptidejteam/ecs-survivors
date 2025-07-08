//
// Created by laurent on 07/07/25.
//

#ifndef COLLISION_DETECTION_RELATIONSHIP_SPATIAL_HASHING_SYSTEM_H
#define COLLISION_DETECTION_RELATIONSHIP_SPATIAL_HASHING_SYSTEM_H

#include <flecs.h>

#include "collision_detection_system.h"
#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace physics::systems {
inline void collision_detection_relationship_spatial_hashing_system(flecs::iter &it, size_t i, CollisionRecordList &list, SpatialHashingGrid &grid,
                         GridCell &cell) {

                    flecs::entity current_cell = it.entity(i);
                    auto cur_q = it.world().query_builder<const core::Position2D, const Collider>().with<ContainedIn>(current_cell).filter();

                    std::vector<CollisionRecord> collisions;
                    for (int offset_y = -1; offset_y <= 1; offset_y++) {
                        for (int offset_x = -1; offset_x <= 1; offset_x++) {
                            int x = cell.x + offset_x;
                            int y = cell.y + offset_y;
                            if (!grid.cells.contains(std::make_pair(x, y))) continue;

                            auto pair = std::make_pair(x, y);
                            flecs::entity neighbour_cell = grid.cells[pair];

                            auto other_q = it.world().query_builder<const core::Position2D, const Collider>().with<ContainedIn>(neighbour_cell).filter();

                            cur_q.each([&](
                            flecs::iter &self_it, size_t self_i, const core::Position2D &pos,
                            const Collider &collider) {
                                    flecs::entity self = self_it.entity(self_i);
                                    other_q.each([&](flecs::iter &other_it, size_t other_i,
                                                                const core::Position2D &other_pos,
                                                                const Collider &other_collider) {
                                        flecs::entity other = other_it.entity(other_i);
                                        if (other.id() <= self.id()) return;

                                        if ((collider.collision_filter & other_collider.collision_type) == none) return;

                                        Rectangle self_rec = {
                                            pos.value.x + collider.bounds.x, pos.value.y + collider.bounds.y,
                                            collider.bounds.width,
                                            collider.bounds.height
                                        };
                                        Rectangle other_rec = {
                                            other_pos.value.x + other_collider.bounds.x,
                                            other_pos.value.y + other_collider.bounds.y,
                                            other_collider.bounds.width, other_collider.bounds.height
                                        };

                                        if (CheckCollisionRecs(self_rec, other_rec)) {
                                            collisions.push_back({self, other});
                                        }
                                    });
                                });
                        }
                    }
                    if (collisions.empty()) return;
                    list_mutex.lock();
                    list.records.insert(list.records.end(), collisions.begin(), collisions.end());
                    list_mutex.unlock();
                }
}
#endif //COLLISION_DETECTION_RELATIONSHIP_SPATIAL_HASHING_SYSTEM_H
