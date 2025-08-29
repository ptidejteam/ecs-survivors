//
// Created by laurent on 07/07/25.
//

#ifndef UPDATE_GRID_SYSTEM_H
#define UPDATE_GRID_SYSTEM_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include "modules/engine/physics/components.h"
#include "modules/engine/rendering/components.h"


namespace physics::systems {
    inline void update_grid_system(flecs::iter &it, size_t i, SpatialHashingGrid &grid, rendering::TrackingCamera &cam,
                         core::GameSettings &settings, GridCell &cell) {
        grid.offset = cam.camera.target - Vector2{
            settings.window_width / 2.0f, settings.window_height / 2.0f
        };

        cell.entities.clear();
    }
}
#endif //UPDATE_GRID_SYSTEM_H
