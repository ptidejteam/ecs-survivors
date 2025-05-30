//
// Created by laurent on 30/05/25.
//

#ifndef DEBUG_GRID_SYSTEM_H
#define DEBUG_GRID_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

namespace debug::systems {
    inline void debug_grid_system(flecs::iter &iter) {
        GuiGrid({0, 0, (float) GetScreenWidth(), (float) GetScreenHeight()}, "grid", 32, 1, nullptr);
    }
}
#endif //DEBUG_GRID_SYSTEM_H
