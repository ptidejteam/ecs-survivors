//
// Created by laurent on 29/05/25.
//

#ifndef DETERMINE_VISIBLE_ENTITIES_SYSTEM_H
#define DETERMINE_VISIBLE_ENTITIES_SYSTEM_H

namespace rendering::systems {
    inline void determine_visible_entities_system(flecs::entity e, const core::Position2D &pos,
                                                  const Renderable &renderable,
                                                  const Settings &settings,
                                                  const TrackingCamera& camera) {
        //if (!camera.target.is_alive()) return;
        if (pos.value.x - camera.camera.target.x > settings.window_width * 1.05f + renderable.texture.width - camera.camera.offset.x|| pos.value.x - camera.camera.target.x + settings.window_width * 0.05f < -renderable.texture.
            width - camera.camera.offset.x ||
            pos.value.y - camera.camera.target.y > settings.window_height * 1.05f + renderable.texture.height - camera.camera.offset.y || pos.value.y - camera.camera.target.y + settings.window_height * 0.05f < -renderable.texture
            .height - camera.camera.offset.y) {
            e.remove<Visible>();
        } else if (!e.has<Visible>()) {
            e.add<Visible>();
        }
    }
}
#endif //DETERMINE_VISIBLE_ENTITIES_SYSTEM_H
