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
                         core::GameSettings &settings, GridCell &cell) {
        auto player = it.world().lookup("player");
        Vector2 pos = player ? player.get<core::Position2D>().value : Vector2(0.0f, 0.0f);
        grid.offset = pos - Vector2{
            settings.window_width / 2.0f, settings.window_height / 2.0f
        };

        cell.entities.clear();
    }
}
#endif //UPDATE_GRID_SYSTEM_H
