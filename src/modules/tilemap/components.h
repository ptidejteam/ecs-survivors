//
// Created by laurent on 03/06/25.
//

#ifndef TILEMAP_COMPONENTS_H
#define TILEMAP_COMPONENTS_H
#include <string>
#include <raylib.h>
#include <raymath.h>
#include "tmxlite/Types.hpp"

namespace tilemap {

    struct Tilemap {
        std::string tmx_file_path;
        float scale;
    };

    struct TilemapTileset {
        Texture texture;
    };

    struct TilemapLayer {
    };

    struct TilemapLayerTile {
        flecs::entity_t tileset;
        Rectangle source;
        Rectangle destination;
        float rotation;
    };
}
#endif //TILEMAP_COMPONENTS_H
