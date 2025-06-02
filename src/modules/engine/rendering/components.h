//
// Created by laure on 1/27/2025.
//

#ifndef RENDERING_COMPONENTS_H
#define RENDERING_COMPONENTS_H

#include <raylib.h>

namespace rendering {
    struct Priority {int priority;};
    struct Visible {};

    struct Renderable {
        Texture2D texture;
        Vector2 draw_offset;
        float scale;
        Color tint;
    };

    struct Rotation {
        float angle;
    };

    struct ProgressBar {
        float min_val;
        float max_val;
        float current_val;
        Rectangle rectangle;
    };
}

#endif //RENDERING_COMPONENTS_H
