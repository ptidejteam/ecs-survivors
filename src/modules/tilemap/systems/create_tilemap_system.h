//
// Created by laurent on 03/06/25.
//

#ifndef CREATE_TILEMAP_SYSTEM_H
#define CREATE_TILEMAP_SYSTEM_H
#include <flecs.h>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "modules/tilemap/components.h"

namespace tilemap::systems {
    inline void create_tilemap_system(flecs::entity e, const Tilemap &tilemap) {
        if (tmx::Map map; map.load(tilemap.tmx_file_path)) {
            const auto &tilesets = map.getTilesets();
            std::vector<std::tuple<flecs::entity, tmx::Tileset> > tileset_first_gids;
            for (const auto &tileset: tilesets) {
                flecs::entity tilemap_tileset = e.world().entity().child_of(e)
                        .set<TilemapTileset>({
                            LoadTexture(tileset.getImagePath().c_str())
                        });
                tileset_first_gids.emplace_back(tilemap_tileset, tileset);
            }

            for (const auto &layers = map.getLayers(); const auto &layer: layers) {
                if (layer->getType() == tmx::Layer::Type::Tile) {
                    const auto &tileLayer = layer->getLayerAs<tmx::TileLayer>();
                    //read out tile layer properties etc...
                    tmx::Vector2u size = tileLayer.getSize();

                    flecs::entity layer = e.world().entity(tileLayer.getName().c_str()).child_of(e).add<
                        TilemapLayer>();

                    const auto &tiles = tileLayer.getTiles();
                    for (int x = 0; x < size.x; x++) {
                        for (int y = 0; y < size.y; y++) {
                            int index = y * size.x + x;
                            int tile_id = tiles[index].ID;
                            auto tileset = tileset_first_gids[0];
                            for (int i = 0; i < tileset_first_gids.size(); i++) {
                                int first_id = std::get<1>(tileset_first_gids[i]).getFirstGID();
                                int last_id = std::get<1>(tileset_first_gids[i]).getLastGID();
                                if (tile_id >= first_id && tile_id < last_id) {
                                    tileset = tileset_first_gids[i];
                                    break;
                                }
                            }

                            auto tile = std::get<1>(tileset).getTile(tile_id);
                            if (tile == nullptr) continue;
                            if (tiles[index].ID == 0) continue;

                            int width = tile->imageSize.x;
                            int height = tile->imageSize.y;
                            float rotation = 0.0f;
                            if (tiles[index].flipFlags & tmx::TileLayer::FlipFlag::Diagonal) {
                                rotation = -90;
                                height = -height;
                            }
                            if (tiles[index].flipFlags & tmx::TileLayer::FlipFlag::Horizontal) {
                                width = -width;
                            }
                            if (tiles[index].flipFlags & tmx::TileLayer::FlipFlag::Vertical) {
                                height = -height;
                            }

                            e.world().entity().child_of(layer)
                                    .set<TilemapLayerTile>({
                                        std::get<0>(tileset),
                                        {
                                            (float) tile->imagePosition.x,
                                            (float) tile->imagePosition.y,
                                            (float) width,
                                            (float) height
                                        },
                                        {
                                            (float) x * tilemap.scale * tile->imageSize.x,
                                            (float) y * tilemap.scale * tile->imageSize.y,
                                            (float) tile->imageSize.x * tilemap.scale,
                                            (float) tile->imageSize.y * tilemap.scale
                                        },
                                        rotation
                                    });
                        }
                    }
                }
            }
        }
    }
}
#endif //CREATE_TILEMAP_SYSTEM_H
