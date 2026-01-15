//
// Created by laurent on 30/05/25.
//

#ifndef DEBUG_FPS_SYSTEM_H
#define DEBUG_FPS_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

namespace debugging::systems {
    inline void debug_fps_system(flecs::iter &iter) {
        DrawRectangleRec({0, 10, 225, 20}, DARKGRAY);
        DrawFPS(10, 10);
    }
}
#endif //DEBUG_FPS_SYSTEM_H
