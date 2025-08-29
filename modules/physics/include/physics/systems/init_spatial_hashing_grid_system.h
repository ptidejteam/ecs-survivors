//
// Created by laurent on 07/07/25.
//

#ifndef INIT_SPATIAL_HASHING_GRID_SYSTEM_H
#define INIT_SPATIAL_HASHING_GRID_SYSTEM_H

#include <flecs.h>

#include "core/components.h"
#include "physics/components.h"
#include "physics/physics_module.h"

namespace physics::systems {
    inline void init_spatial_hashing_grid_system(flecs::iter &it, size_t i, SpatialHashingGrid &hashing_grid,
                                                 core::GameSettings &settings) {
        for (int y = -1; y < std::ceil((float) settings.window_height / (float) hashing_grid.cell_size) + 1;
             y++) {
            for (int x = -1; x < std::ceil(settings.window_width / hashing_grid.cell_size) + 1; x++) {
                flecs::entity e = it.world().entity().set<GridCell>({x, y});
                hashing_grid.cells[std::make_pair(x, y)] = e;
            }
        }
    }
}
#endif //INIT_SPATIAL_HASHING_GRID_SYSTEM_H
