//
// Created by laurent on 07/07/25.
//

#ifndef COLLISION_DETECTION_SPATIAL_HASHING_SYSTEM_H
#define COLLISION_DETECTION_SPATIAL_HASHING_SYSTEM_H

#include <flecs.h>

#include "collision_detection_system.h"
#include "core/components.h"
#include "physics/components.h"

namespace physics::systems {
    inline void collision_detection_spatial_hashing_system(flecs::entity e, CollisionRecordList &list, SpatialHashingGrid &grid, GridCell &cell) {
                    std::vector<CollisionRecord> collisions;
                    for (int offset_y = -1; offset_y <= 1; offset_y++) {
                        for (int offset_x = -1; offset_x <= 1; offset_x++) {
                            int x = cell.x + offset_x;
                            int y = cell.y + offset_y;
                            if (!grid.cells.contains(std::make_pair(x, y))) continue;

                            const GridCell neighbour = grid.cells[std::make_pair(x, y)].get<GridCell>();

                            for (int i = 0; i < cell.entities.size(); i++) {
                                flecs::entity self = cell.entities[i];
                                const core::Position2D pos = cell.entities[i].get<core::Position2D>();
                                const Collider collider = cell.entities[i].get<Collider>();
                                for (int j = 0; j < neighbour.entities.size(); j++) {
                                    flecs::entity other = neighbour.entities[j];
                                    if (self.id() <= other.id()) continue;

                                    const core::Position2D other_pos = neighbour.entities[j].get<core::Position2D>();
                                    const Collider other_collider = neighbour.entities[j].get<Collider>();
                                    if ((collider.collision_filter & other_collider.collision_type) == none) continue;

                                    collisions.push_back({self, other});

                                }
                            }
                        }
                    }
                    if (collisions.empty()) return;
                    list_mutex.lock();
                    list.records.insert(list.records.end(), collisions.begin(), collisions.end());
                    list_mutex.unlock();
                }
}
#endif //COLLISION_DETECTION_SPATIAL_HASHING_SYSTEM_H
