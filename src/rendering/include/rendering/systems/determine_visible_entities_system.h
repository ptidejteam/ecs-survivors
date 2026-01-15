//
// Created by laurent on 29/05/25.
//

#ifndef DETERMINE_VISIBLE_ENTITIES_SYSTEM_H
#define DETERMINE_VISIBLE_ENTITIES_SYSTEM_H

namespace rendering::systems {
    inline void determine_visible_entities_system(flecs::iter& iter, size_t i, const core::Position2D &pos,
                                                  const Renderable &renderable,
                                                  const TrackingCamera& camera) {

        flecs::entity e = iter.entity(i);

        //TODO: rewrite the visible logic

        e.add<Visible>();

        return;

        //if (!camera.target.is_alive()) return;
        if (pos.value.x - camera.camera.target.x > camera.camera.offset.x * 1.05f + renderable.texture.width|| pos.value.x - camera.camera.target.x + camera.camera.offset.x * 0.05f < -renderable.texture.
            width ||
            pos.value.y - camera.camera.target.y > camera.camera.offset.y * 1.05f + renderable.texture.height - camera.camera.offset.y || pos.value.y - camera.camera.target.y + camera.camera.offset.y * 0.05f < -renderable.texture
            .height) {
            e.remove<Visible>();
        } else if (!e.has<Visible>()) {
            e.add<Visible>();
        }
    }
}
#endif //DETERMINE_VISIBLE_ENTITIES_SYSTEM_H
