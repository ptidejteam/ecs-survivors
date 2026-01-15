//
// Created by laurent on 30/05/25.
//

#ifndef DEBUG_GRID_SYSTEM_H
#define DEBUG_GRID_SYSTEM_H

#include "physics/components.h"

namespace debugging::systems {
    inline void debug_grid_system(physics::SpatialHashingGrid& grid, physics::GridCell& cell) {
        DrawRectangleLines(grid.offset.x + cell.x * grid.cell_size, grid.offset.y + cell.y * grid.cell_size, grid.cell_size, grid.cell_size, ORANGE);
    }
}
#endif //DEBUG_GRID_SYSTEM_H
