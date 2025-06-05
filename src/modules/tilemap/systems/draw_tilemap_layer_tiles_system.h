//
// Created by laurent on 03/06/25.
//

#ifndef DRAW_TILEMAP_LAYER_TILES_SYSTEM_H
#define DRAW_TILEMAP_LAYER_TILES_SYSTEM_H
#include <flecs.h>

#include "modules/tilemap/components.h"

namespace tilemap::systems {
    inline void draw_tilemap_layer_tiles_system(flecs::entity e, TilemapLayerTile &tile) {
        const TilemapTileset *mytileset = e.world().get_alive(tile.tileset).get<TilemapTileset>();
        DrawTexturePro(
            mytileset->texture,
            tile.source,
            tile.destination,
            {
                tile.destination.width / 2,
                tile.destination.height / 2
            },
            tile.rotation, WHITE);
    }

    inline void draw_tilemap_layers_system(flecs::entity e, TilemapLayer &layer) {
        DrawTexturePro(
            layer.texture,
            {0, 0, (float) layer.texture.width, -(float) layer.texture.height}, // <-- flip Y
            {0, 0, (float) layer.texture.width, (float) layer.texture.height},
            {0, 0},
            0.0f,
            WHITE
        );
    }
}

#endif //DRAW_TILEMAP_LAYER_TILES_SYSTEM_H
