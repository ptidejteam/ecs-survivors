//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_HEALTH_BAR_SYSTEM_H
#define DRAW_HEALTH_BAR_SYSTEM_H

namespace rendering::systems {
    // TODO dont like having gameplay since it's outside of core modules
    inline void draw_health_bar_system(ProgressBar bar, Rectangle rec,
                                       const core::Position2D &pos,
                                       const Renderable &renderable) {
        if (bar.max_val - bar.current_val <= 0.05f) return;
        GuiProgressBar(
            {
                pos.value.x - rec.width / 2.f,
                pos.value.y - rec.height - renderable.texture.height / 2.0f * renderable.scale,
                rec.width,
                rec.height,
            }, "", "", &bar.current_val, 0,
            bar.max_val);
    }
}
#endif //DRAW_HEALTH_BAR_SYSTEM_H
