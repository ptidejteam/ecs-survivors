//
// Created by Laurent Voisard on 12/20/2024.
//

#include "game.h"
#include <iostream>
#include <ostream>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

#include <thread>

#include "modules/ai/ai_module.h"
#include "modules/ai/components.h"
#include "modules/engine/core/components.h"
#include "modules/engine/core/core_module.h"
#include "modules/engine/input/components.h"
#include "modules/engine/input/input_module.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/physics/physics_module.h"
#include "modules/player/player_module.h"
#include "raylib.h"
#include "modules/debug/debug_module.h"

#include "modules/engine/rendering/components.h"
#include "modules/engine/rendering/rendering_module.h"
#include "modules/gameplay/components.h"
#include "modules/gameplay/gameplay_module.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

Game::Game(const char *windowName, int windowWidth, int windowHeight) : m_world(flecs::world()),
                                                                        m_windowName(windowName),
                                                                        m_windowHeight(windowHeight),
                                                                        m_windowWidth(windowWidth) {
    // Raylib window
    //#ifndef EMSCRIPTEN
    // web has an scaling issue with the cursor
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    //#endif


    InitWindow(m_windowWidth, m_windowHeight, m_windowName.c_str());
    // SetTargetFPS(GetMonitorRefreshRate(0));

#ifndef EMSCRIPTEN
    // use the flecs explorer when not on browser
    m_world.import<flecs::stats>();
    m_world.set<flecs::Rest>({});
    m_world.set_threads(static_cast<int>(std::thread::hardware_concurrency()));
#endif

    m_world.import<core::CoreModule>();
    m_world.import<input::InputModule>();
    m_world.import<rendering::RenderingModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();
    m_world.import<gameplay::GameplayModule>();
    m_world.import<debug::DebugModule>();

    m_world.set<core::GameSettings>({
        m_windowName,
        m_windowWidth,
        m_windowHeight,
        m_windowWidth,
        m_windowHeight
    });
    m_world.add<physics::CollisionRecordList>();

    flecs::entity player = m_world.entity("player")
            .set<core::Tag>({"player"})
            .set<core::Position2D>({GetScreenWidth() / 2.f, GetScreenHeight() / 2.f})
            .set<core::Speed>({150})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<physics::Collider>({
                24,
                true,
                physics::CollisionFilter::player,
                physics::player_filter
            })
            .set<rendering::Priority>({2})
            .set<rendering::Renderable>({
                LoadTexture("../resources/player.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            })
            .set<gameplay::Health>({150, 150})
            .set<gameplay::RegenHealth>({2.5f});

    m_world.entity("dagger attack").child_of(player)
            .add<gameplay::Projectile>()
            .set<gameplay::Attack>({"projectile", "enemy"})
            .set<gameplay::Cooldown>({1.0f, 1})
            .add<gameplay::CooldownCompleted>()
            .set<gameplay::MultiProj>({3, 30.f, 150.f, 30.f})
            .set<core::Speed>({150.f});

    m_world.prefab("projectile")
            .add<gameplay::Projectile>()
            .set<gameplay::Attack>({"projectile", "enemy"})
            .set<gameplay::Chain>({
                6,
                std::unordered_set<int>()
            })
            .set<gameplay::Split>({std::unordered_set<int>()})
            .set<gameplay::Damage>({2})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::Collider>({
                24,
                false,
                physics::CollisionFilter::player,
                physics::player_filter
            })
            .set<rendering::Priority>({1})
            .set<rendering::Renderable>({
                LoadTexture("../resources/dagger.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            })
            .set<core::DestroyAfterTime>({5});

    auto hori = m_world.entity("player_horizontal_input").child_of(player).set<input::InputHorizontal>({});
    m_world.entity().child_of(hori).set<input::KeyBinding>({KEY_A, -1});
    m_world.entity().child_of(hori).set<input::KeyBinding>({KEY_D, 1});
    m_world.entity().child_of(hori).set<input::KeyBinding>({KEY_LEFT, -1});
    m_world.entity().child_of(hori).set<input::KeyBinding>({KEY_RIGHT, 1});

    auto vert = m_world.entity("player_vertical_input").child_of(player).set<input::InputVertical>({});
    m_world.entity().child_of(vert).set<input::KeyBinding>({KEY_W, -1});
    m_world.entity().child_of(vert).set<input::KeyBinding>({KEY_S, 1});
    m_world.entity().child_of(vert).set<input::KeyBinding>({KEY_UP, -1});
    m_world.entity().child_of(vert).set<input::KeyBinding>({KEY_DOWN, 1});

    flecs::entity enemy = m_world.prefab("enemy")
            .set<core::Tag>({"enemy"})
            .set<core::Position2D>({800, 400})
            .set<core::Speed>({25})
            .set<gameplay::Health>({10, 10})
            .set<gameplay::Damage>({1})
            .add<ai::Target>(player)
            .add<ai::FollowTarget>()
            .set<ai::StoppingDistance>({16.0})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<physics::Collider>({
                24,
                true,
                physics::CollisionFilter::enemy,
                physics::enemy_filter
            })
            .set<rendering::Renderable>({
                LoadTexture("../resources/ghost.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            })
            .set<rendering::Priority>({0});

    m_world.entity("enemy_spawner")
            .set<gameplay::Spawner>({enemy});
}


void Game::run() {
    // ON START
    m_world.progress();

    Texture2D text;
    tmx::Map map;

    //std::vector<>
    if (map.load("../resources/tiled/maps/sampleMap.tmx")) {
        std::cout << "hurray" << std::endl;
        const auto &layers = map.getLayers();
        for (const auto &layer: layers) {
            if (layer->getType() == tmx::Layer::Type::Object) {
                const auto &objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
                const auto &objects = objectLayer.getObjects();
                for (const auto &object: objects) {
                    //do stuff with object properties
                }
            } else if (layer->getType() == tmx::Layer::Type::Tile) {
                const auto &tileLayer = layer->getLayerAs<tmx::TileLayer>();
                //read out tile layer properties etc...
            }
        }

        const auto &tilesets = map.getTilesets();
        for (const auto &tileset: tilesets) {
            std::cout << tileset.getImagePath() << std::endl;
            text = LoadTexture(tileset.getImagePath().c_str());
            //read out tile set properties, load textures etc...
        }
    }


    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        m_world.progress(GetFrameTime());
        const auto &tilesets = map.getTilesets();
        for (const auto &tileset: tilesets) {
            const auto &layers = map.getLayers();
            for (const auto &layer: layers) {
                if (layer->getType() == tmx::Layer::Type::Object) {
                    const auto &objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
                    const auto &objects = objectLayer.getObjects();
                    for (const auto &object: objects) {
                        //do stuff with object properties
                    }
                } else if (layer->getType() == tmx::Layer::Type::Tile) {
                    if (layer->getName() != "Dungeon") continue;
                    const auto &tileLayer = layer->getLayerAs<tmx::TileLayer>();
                    //read out tile layer properties etc...
                    tmx::Vector2u size = layer->getSize();

                    const auto &tiles = tileLayer.getTiles();
                    for (int x = 0; x < size.x; x++) {
                        for (int y = 0; y < size.y; y++) {
                            int index = y * size.x + x;
                            auto tile = tileset.getTile(tiles[index].ID);

                            float rotation = 0;
                            //if (tiles[index].flipFlags == tmx::TileLayer::FlipFlag::Horizontal)
                            //    rotation = 180.f;

                            DrawTexturePro(
                                text,
                                {
                                    (float) tile->imagePosition.x,
                                    (float) tile->imagePosition.y,
                                    (float) tile->imageSize.x,
                                    (float) tile->imageSize.y
                                },
                                {
                                    (float) 0 + x * 4.f * tile->imageSize.x,
                                    (float) 0 + y * 4.f * tile->imageSize.y,
                                    (float) tile->imageSize.x * 4.f,
                                    (float) tile->imageSize.y * 4.f
                                }, {0, 0}, rotation, WHITE);
                        }
                    }
                }
            }
        }

        // for (const auto &tileset: tilesets) {
        //     //std::cout << tileset.getImagePath() << std::endl;
        //     for (const auto &tile: tileset.getTiles()) {
        //         //std::cout << tile.ID << std::endl;
        //         //tile.imagePosition
        //         DrawTextureRec(
        //             text, {
        //                 (float) tile.imagePosition.x,
        //                 (float) tile.imagePosition.y,
        //                 (float) tile.imageSize.x,
        //                 (float) tile.imageSize.y
        //             }, {
        //                 (float) tile.imagePosition.x,
        //                 (float) tile.imagePosition.y
        //             }, WHITE);
        //     }
        //     //read out tile set properties, load textures etc...
        // }
        //DrawTexture(text, 0,0, WHITE);
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
