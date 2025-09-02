//
// Created by laure on 1/27/2025.
//

#ifndef RENDERING_COMPONENTS_H
#define RENDERING_COMPONENTS_H

#include <flecs.h>
#include <raylib.h>
#include <core/components.h>

namespace rendering {
    struct Priority {int priority;};
    struct Visible {};

    struct Renderable {
        Texture2D texture;
        Vector2 draw_offset;
        float scale;
        Color tint;
    };

    struct Settings {
        std::string window_name;
        int initial_width;
        int initial_height;
        int window_width;
        int window_height;
    };

    struct TrackingCamera {
        flecs::entity target;
        Camera2D camera {0};
    };

    struct Viewport {
        RenderTexture render_target;
        Rectangle rect;
    };

    struct Rotation {
        float angle;
    };

    struct ProgressBar {
        float min_val;
        float max_val;
        float current_val;
    };
}

#endif //RENDERING_COMPONENTS_H
