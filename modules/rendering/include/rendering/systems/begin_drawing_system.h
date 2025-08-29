//
// Created by laurent on 29/05/25.
//

#ifndef BEGIN_DRAWING_SYSTEM_H
#define BEGIN_DRAWING_SYSTEM_H

namespace rendering::systems {
    inline void begin_drawing_system(flecs::iter& it) {
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    }
}

#endif //BEGIN_DRAWING_SYSTEM_H
