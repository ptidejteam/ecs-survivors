//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_HEALTH_BAR_SYSTEM_H
#define DRAW_HEALTH_BAR_SYSTEM_H

namespace rendering {
    // TODO dont like having gameplay since it's outside of core modules
    inline void draw_health_bar_system(const HealthBar bar, gameplay::Health &health,
                                       const core::Position2D &pos,
                                       const Renderable &renderable) {
        if (health.max - health.value <= 0.05f) return;
        GuiProgressBar(
            {
                pos.value.x - bar.rectangle.width / 2.f,
                pos.value.y - bar.rectangle.height - renderable.texture.height / 2.0f * renderable.scale,
                bar.rectangle.width,
                bar.rectangle.height,
            }, "", "", &health.value, 0,
            health.max);
    }
}
#endif //DRAW_HEALTH_BAR_SYSTEM_H
