//
// Created by laurent on 07/07/25.
//

#ifndef COLLISION_DETECTION_SPATIAL_HASHING_SYSTEM_H
#define COLLISION_DETECTION_SPATIAL_HASHING_SYSTEM_H

#include <flecs.h>

#include "collision_detection_system.h"
#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

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
                                if (!self.is_alive()) continue;
                                const core::Position2D pos = cell.entities[i].get<core::Position2D>();
                                const Collider collider = cell.entities[i].get<Collider>();
                                for (int j = 0; j < neighbour.entities.size(); j++) {
                                    flecs::entity other = neighbour.entities[j];
                                    if (!other.is_alive()) continue;
                                    const core::Position2D other_pos = neighbour.entities[j].get<core::Position2D>();
                                    const Collider other_collider = neighbour.entities[j].get<Collider>();
                                    if (self.id() <= other.id()) continue;

                                    if ((collider.collision_filter & other_collider.collision_type) == none) continue;

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
                                        //std::cout << "Collision detected" << std::endl;
                                        collisions.push_back({self, other});
                                    }
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
