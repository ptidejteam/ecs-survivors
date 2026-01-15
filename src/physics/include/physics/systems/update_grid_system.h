//
// Created by laurent on 07/07/25.
//

#ifndef UPDATE_GRID_SYSTEM_H
#define UPDATE_GRID_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include "physics/components.h"


namespace physics::systems {
    inline void update_grid_system(flecs::iter &it, size_t i, SpatialHashingGrid &grid,
                         Settings &settings, GridCell &cell) {
        auto player = it.world().lookup("player");
        Vector2 pos = player != 0 ? player.get<core::Position2D>().value : Vector2(0.0f, 0.0f);
        grid.offset = pos - Vector2{
            settings.world_bounds.x / 2.f, settings.world_bounds.y / 2.f
        };

        cell.entities.clear();
    }
}
#endif //UPDATE_GRID_SYSTEM_H
