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

#include "core/components.h"
#include "physics/components.h"
#include "physics/physics_module.h"
#include "rendering/components.h"
#include "tilemap/components.h"

#include "../../../../core/include/core/logger.h"

namespace tilemap::systems {
    inline void create_tilemap_system(flecs::entity e, const Tilemap &tilemap) {
        LOG_INFO(core::Tilemap, "tilemap creation");
        if (tmx::Map map; map.load(tilemap.tmx_file_path)) {
            const auto &tilesets = map.getTilesets();
            std::vector<std::tuple<flecs::entity, tmx::Tileset, Texture> > tileset_first_gids;
            for (const auto &tileset: tilesets) {
                Texture2D t = LoadTexture(tileset.getImagePath().c_str());
                flecs::entity tilemap_tileset = e.world().entity().child_of(e)
                        .set<TilemapTileset>({
                            t
                        });
                tileset_first_gids.emplace_back(tilemap_tileset, tileset, t);
            }
            std::vector<bool> collision_map(map.getTileCount().x * map.getTileCount().y);

            for (const auto &layers = map.getLayers(); const auto &layer: layers) {
                if (layer->getType() == tmx::Layer::Type::Tile) {
                    RenderTexture2D layer_texture = LoadRenderTexture(
                        layer->getSize().x * map.getTileSize().x * tilemap.scale,
                        layer->getSize().y * map.getTileSize().y * tilemap.scale);
                    BeginTextureMode(layer_texture);
                    ClearBackground(BLANK);
                    const auto &tileLayer = layer->getLayerAs<tmx::TileLayer>();
                    //read out tile layer properties etc...
                    tmx::Vector2u size = tileLayer.getSize();

                    flecs::entity layer_entity = e.world().entity(tileLayer.getName().c_str()).child_of(e);

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

                            Rectangle source{
                                (float) tile->imagePosition.x,
                                (float) tile->imagePosition.y,
                                (float) width,
                                (float) height
                            };
                            Rectangle destination{
                                (float) x * tilemap.scale * tile->imageSize.x,
                                (float) y * tilemap.scale * tile->imageSize.y,
                                (float) tile->imageSize.x * tilemap.scale,
                                (float) tile->imageSize.y * tilemap.scale
                            };

                            Vector2 origin{
                                destination.width / 2.f,
                                destination.height / 2.f
                            };


                            flecs::entity tile_entity = e.world().entity().child_of(layer_entity)
                                    .set<TilemapLayerTile>({
                                        std::get<0>(tileset),
                                        source,
                                        destination,
                                        rotation,
                                    });

                            if (!tile->properties.empty()) {
                                for (int i = 0; i < tile->properties.size(); i++) {
                                    if (tile->properties[i].getName() == "collide") {
                                        if (tile->properties[i].getBoolValue()) {
                                            collision_map[index] = true;
                                        }
                                    }
                                }
                            }

                            DrawTexturePro(
                                std::get<2>(tileset),
                                source,
                                destination,
                                origin,
                                rotation,
                                WHITE
                            );
                        }
                    }
                    EndTextureMode();
                    //
                    RenderTexture2D inverted = LoadRenderTexture(
                        layer->getSize().x * map.getTileSize().x * tilemap.scale,
                        layer->getSize().y * map.getTileSize().y * tilemap.scale);
                    BeginTextureMode(inverted);
                    DrawTexture(layer_texture.texture, 0, 0,WHITE);
                    EndTextureMode();
                    layer_entity
                            .set<rendering::Renderable>({
                                inverted.texture, {0, 0}, 1.0f, WHITE
                            })
                            .set<rendering::Priority>({0});
                }
            }

            std::vector<Rectangle> merged_colliders;
            for (int y = 0; y < map.getTileCount().y; ++y) {
                for (int x = 0; x < map.getTileCount().x; ++x) {
                    int current_index = y * map.getTileCount().x + x;

                    // If this tile is not a collision or has already been merged, skip it
                    if (!collision_map[current_index]) {
                        continue;
                    }

                    // Found an unmerged collision tile, start a new rectangle from here
                    int rect_x = x;
                    int rect_y = y;
                    int rect_width = 1;
                    int rect_height = 1;

                    // 1. Extend to the right to find maximum width
                    // While the next tile to the right exists and is a collision
                    while (rect_x + rect_width < map.getTileCount().x &&
                           collision_map[y * map.getTileCount().x + (rect_x + rect_width)]) {
                        rect_width++;
                    }

                    // 2. Extend downwards to find maximum height
                    // We need to check if the entire proposed row below is clear (collidable)
                    bool can_extend_down = true;
                    while (can_extend_down && rect_y + rect_height < map.getTileCount().y) {
                        // Check the entire row at current_y + rect_height for collisions within current rect_width
                        for (int current_rect_x = rect_x; current_rect_x < rect_x + rect_width; ++current_rect_x) {
                            if (!collision_map[(rect_y + rect_height) * map.getTileCount().x + current_rect_x]) {
                                can_extend_down = false;
                                break; // This row cannot be fully extended
                            }
                        }
                        if (can_extend_down) {
                            rect_height++;
                        }
                    }

                    // Now we have the largest possible rectangle starting at (rect_x, rect_y)
                    // Mark all tiles within this rectangle as processed in the collision_map
                    for (int mark_y = rect_y; mark_y < rect_y + rect_height; ++mark_y) {
                        for (int mark_x = rect_x; mark_x < rect_x + rect_width; ++mark_x) {
                            collision_map[mark_y * map.getTileCount().x + mark_x] = false;
                        }
                    }

                    // Add the new merged rectangle to our list
                    merged_colliders.push_back({
                        rect_x * map.getTileSize().x * tilemap.scale - map.getTileSize().x * tilemap.scale / 2.0f,
                        rect_y * map.getTileSize().y * tilemap.scale - map.getTileSize().y * tilemap.scale / 2.0f,
                        rect_width * map.getTileSize().x * tilemap.scale,
                        rect_height * map.getTileSize().y * tilemap.scale
                    });
                }
            }

            for (auto col: merged_colliders) {
                e.world().entity()
                        .set<core::Position2D>({col.x, col.y})
                        .set<physics::Collider>({
                            false,
                            true,
                            {0, 0, col.width, col.height},
                            physics::environment, physics::environment_filter,
                            physics::ColliderType::Box
                        })
                        .add<physics::BoxCollider>()
                        .add<physics::StaticCollider>();
            }
        }
        LOG_INFO(core::Tilemap, "tilemap creation ended");
    }
}
#endif //CREATE_TILEMAP_SYSTEM_H
