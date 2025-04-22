//
// Created by laure on 1/27/2025.
//

#ifndef RENDERING_COMPONENTS_H
#define RENDERING_COMPONENTS_H

namespace rendering {
    struct Circle {
        float radius;
    };

    struct Priority {};
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

    struct HealthBar {
        Rectangle rectangle;
    };
}

#endif //RENDERING_COMPONENTS_H
