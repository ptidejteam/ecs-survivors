//
// Created by laurent on 29/05/25.
//

#ifndef DETERMINE_VISIBLE_ENTITIES_SYSTEM_H
#define DETERMINE_VISIBLE_ENTITIES_SYSTEM_H

namespace rendering {
    inline void determine_visible_entities_system(flecs::entity e, const core::Position2D &pos,
                                                  const Renderable &renderable,
                                                  const core::GameSettings &settings) {
        if (pos.value.x > settings.windowWidth + renderable.texture.width || pos.value.x < -renderable.texture.
            width ||
            pos.value.y > settings.windowHeight + renderable.texture.height || pos.value.y < -renderable.texture
            .height) {
            e.remove<Visible>();
        } else if (!e.has<Visible>()) {
            e.add<Visible>();
        }
    }
}
#endif //DETERMINE_VISIBLE_ENTITIES_SYSTEM_H
