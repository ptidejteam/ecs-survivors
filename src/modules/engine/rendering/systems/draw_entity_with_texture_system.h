//
// Created by laurent on 29/05/25.
//

#ifndef DRAW_ENTITY_WITH_TEXTURE_SYSTEM_H
#define DRAW_ENTITY_WITH_TEXTURE_SYSTEM_H

namespace rendering {
    inline void draw_entity_with_texture_system(const Renderable &renderable, const core::Position2D &position, const Rotation *rotation) {
        Rectangle rec{
            0.0f, 0.0f,
            (float) renderable.texture.width,
            (float) renderable.texture.height
        };

        float scaledWidth = renderable.texture.width * renderable.scale;
        float scaledHeight = renderable.texture.height * renderable.scale;

        Rectangle source{
            position.value.x + renderable.draw_offset.x * renderable.scale,
            position.value.y + renderable.draw_offset.y * renderable.scale,
            scaledWidth,
            scaledHeight
        };

        Vector2 origin = Vector2{
            scaledWidth / 2.0f,
            scaledHeight / 2.0f
        };

        float r = rotation ? rotation->angle : 0.0f;
        DrawTexturePro(renderable.texture, rec, source, origin, r, renderable.tint);
    }
}
#endif //DRAW_ENTITY_WITH_TEXTURE_SYSTEM_H
