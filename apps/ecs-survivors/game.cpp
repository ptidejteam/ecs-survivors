//
// Created by Laurent Voisard on 12/20/2024.
//

#include "game.h"
#include <iostream>
#include <ostream>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

#include <flecs.h>
#include <raylib.h>
#include <thread>

#include "core/components.h"
#include "core/core_module.h"
#include "input/input_module.h"
#include "ai/ai_module.h"
#include "player/player_module.h"
#include "physics/components.h"
#include "physics/physics_module.h"

#include "gameplay/gameplay_module.h"
#include "rendering/components.h"
#include "rendering/rendering_module.h"
#include "tilemap/tilemap_module.h"

#include "gui/gui_module.h"

#include "debugging/debug_module.h"

#include "game_scene.h"

void Game::init() {
    GraphicalApplication::init();

#ifndef EMSCRIPTEN
    // use the flecs explorer when not on browser
    m_world.import <flecs::stats>();
    m_world.set<flecs::Rest>({});
    m_world.set_threads(static_cast<int>(std::thread::hardware_concurrency()));
#endif

    // import modules
    m_world.import <core::CoreModule>();
    m_world.import <input::InputModule>();
    m_world.import <rendering::RenderingModule>();
    m_world.import <gui::GUIModule>();
    m_world.import <physics::PhysicsModule>();
    m_world.import <tilemap::TilemapModule>();
    m_world.import <player::PlayerModule>();
    m_world.import <ai::AIModule>();
    m_world.import <gameplay::GameplayModule>();
    m_world.import <debugging::DebugModule>();


    //configure settings
    m_world.set<rendering::Settings>({m_window_name, m_window_width, m_window_height, m_window_width, m_window_height});
    m_world.set<physics::Settings>({(float) m_window_width, (float) m_window_height});
    m_world.add<physics::CollisionRecordList>();
    m_world.set<physics::SpatialHashingGrid>({48, {0, 0}});
    m_world.set<core::Paused>({false});
    m_world.set<core::EnabledMenus>({0});

    rendering::RenderingModule::main_viewport.remove<rendering::Viewport>();
    rendering::RenderingModule::main_viewport.set<rendering::ScreenViewport>({0,0, 1920, 1080});
    // load whatever you need
    GameScene game_scene;
    game_scene.load(m_world);

}

void Game::run() {
    // ON START
    m_world.progress();



#if defined(EMSCRIPTEN)
    emscripten_set_main_loop_arg(UpdateDrawFrameWeb, this, 0, 1);
#else

    // Main game loop
    while (!WindowShouldClose() && !m_world.has<core::ExitConfirmed>()) // Detect window close button or ESC key
    {
        UpdateDrawFrameDesktop();
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

void Game::UpdateDrawFrameDesktop() { m_world.progress(GetFrameTime()); }

void Game::UpdateDrawFrameWeb(void *game) {
    Game *instance = static_cast<Game *>(game);
    instance->m_world.progress(GetFrameTime());
}
