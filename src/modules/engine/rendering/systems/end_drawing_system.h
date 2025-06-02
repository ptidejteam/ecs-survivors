//
// Created by laurent on 29/05/25.
//

#ifndef END_DRAWING_SYSTEM_H
#define END_DRAWING_SYSTEM_H

namespace rendering::systems {
    inline void end_drawing_system(flecs::iter& it) {
        EndDrawing();
    }
}

#endif //END_DRAWING_SYSTEM_H
