//
// Created by laurent on 07/07/25.
//

#ifndef UPDATE_GRID_ON_WINDOW_RESIZED_SYSTEM_H
#define UPDATE_GRID_ON_WINDOW_RESIZED_SYSTEM_H

#include <cmath>
#include <flecs.h>

#include "init_spatial_hashing_grid_system.h"
#include "modules/engine/core/components.h"
#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void reset_grid(flecs::iter &it, size_t i, SpatialHashingGrid &hashing_grid, core::GameSettings &settings) {
        for (auto cell: hashing_grid.cells) {
            cell.second.destruct();
        }
        hashing_grid.cells.clear();
        init_spatial_hashing_grid_system(it, i, hashing_grid, settings);
    }

    inline void update_grid_on_window_resized_system(flecs::iter &it, size_t i, SpatialHashingGrid &hashing_grid,
                                                     core::GameSettings &settings) {
        if (IsWindowResized()) {
            reset_grid(it, i, hashing_grid, settings);
        }
    }
}
#endif //UPDATE_GRID_ON_WINDOW_RESIZED_SYSTEM_H
