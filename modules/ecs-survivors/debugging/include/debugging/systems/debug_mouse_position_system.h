//
// Created by laurent on 30/05/25.
//

#ifndef DEBUG_MOUSE_POSITION_SYSTEM_H
#define DEBUG_MOUSE_POSITION_SYSTEM_H

#include <flecs.h>
#include <raylib.h>

namespace debugging::systems {
    inline void debug_mouse_position_system(flecs::iter &iter) {
        DrawCircle(GetMouseX(), GetMouseY(), 10, RED);
    }
}
#endif //DEBUG_MOUSE_POSITION_SYSTEM_H
