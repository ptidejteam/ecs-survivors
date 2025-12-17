//
// Created by laurent on 9/12/25.
//

#include "editor.h"

#include "ai/ai_module.h"
#include "core/core_module.h"
#include "debugging/debug_module.h"
#include "editor/editor_module.h"
#include "gameplay/gameplay_module.h"
#include "gui/gui_module.h"
#include "input/input_module.h"
#include "physics/physics_module.h"
#include "player/player_module.h"
#include "rendering/components.h"
#include "rendering/rendering_module.h"
#include "tilemap/tilemap_module.h"

#include "editor/components.h"
#include "rlImGui.h"

#include "game_scene.h"


void Editor::init() {
    GraphicalApplication::init();


#ifndef EMSCRIPTEN
    // use the flecs explorer when not on browser
    m_world.import <flecs::stats>();
    m_world.set<flecs::Rest>({});

#endif

    m_world.import<core::CoreModule>();
    m_world.import<rendering::RenderingModule>();
    m_world.import<gui::GUIModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<tilemap::TilemapModule>();
    m_world.import<input::InputModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();
    m_world.import<gameplay::GameplayModule>();
    m_world.import<debugging::DebugModule>();
    m_world.import<editor::EditorModule>();


    // do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
    // use the native resolution and scale your geometry.

    m_world.set<physics::Settings>({(float) m_window_width, (float) m_window_height});
    m_world.add<physics::CollisionRecordList>();
    m_world.add<physics::SpatialHashingGrid>();
    m_world.get_mut<physics::SpatialHashingGrid>().cell_size = 48;
    m_world.get_mut<physics::SpatialHashingGrid>().offset = {0,0};
    m_world.set<core::Paused>({false});
    m_world.set<core::PausesRequested>({0});

    rendering::RenderingModule::main_viewport
        .remove<rendering::Viewport>();
    rendering::RenderingModule::main_viewport
        .set<rendering::VirtualViewport>({0,0, 1920, 1080})
        .set<editor::Window>({"Game View"});




    // load whatever you need
    GameScene game_scene;
    game_scene.load(m_world);



    m_world.entity().set<editor::Window>({"Hierarchy"}).add<editor::Hierarchy>();
    m_world.entity().set<editor::Window>({"Inspector"}).add<editor::Inspector>();
    m_world.entity().set<editor::Window>({"Console"}).set<editor::Console>({});

    m_world.observer<rendering::VirtualViewport>()
        .event(flecs::OnSet)
        .each([this] (rendering::VirtualViewport vp){
            LOG_INFO(core::Gui, std::format("Canvas Resized ({} x {})", vp.rect.width, vp.rect.height))
            m_world.set<physics::Settings>({vp.rect.width, vp.rect.height, true});
        });
}

void Editor::run() {
    m_world.progress();

    // Main game loop
    while (!WindowShouldClose() && !m_world.has<core::ExitConfirmed>())    // Detect window close button or ESC key, or a quit from the menu
    {
        m_world.progress(GetFrameTime());
    }

    rlImGuiShutdown();

    //delete game_scene;

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}