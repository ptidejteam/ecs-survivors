//
// Created by Laurent Voisard on 12/20/2024.
//

#include "game.h"
#include <iostream>
#include <ostream>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

#include "modules/ai/ai_module.h"
#include "modules/ai/components.h"
#include "modules/engine/core/components.h"
#include "modules/engine/core/core_module.h"
#include "modules/engine/input/components.h"
#include "modules/engine/input/input_module.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/physics/physics_module.h"
#include "modules/engine/rendering/gui/components.h"
#include "modules/player/player_module.h"
#include "raylib.h"
#include "modules/debug/debug_module.h"

#include "modules/engine/rendering/components.h"
#include "modules/engine/rendering/rendering_module.h"
#include "modules/gameplay/components.h"
#include "modules/gameplay/gameplay_module.h"

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
    //m_world.set_threads(4);
    m_world.import<core::CoreModule>();
    m_world.import<input::InputModule>();
    m_world.import<rendering::RenderingModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();
    m_world.import<gameplay::GameplayModule>();
    m_world.import<debug::DebugModule>();


#ifndef EMSCRIPTEN
    // use the flecs explorer when not on browser
    m_world.import<flecs::stats>();
    m_world.set<flecs::Rest>({});
#endif


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
            .set<core::Health>({150, 150})
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
            .add<rendering::Priority>(1)
            .set<rendering::Renderable>({
                LoadTexture("../resources/player.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            })
            .set<gameplay::RegenHealth>({2.5f})
            .set<rendering::HealthBar>({0, 0, 50, 10});

    m_world.entity("dagger attack").child_of(player)
            .add<gameplay::Projectile>()
            .set<core::Attack>({"projectile", "enemy"})
            .set<gameplay::Cooldown>({1.0f, 1})
            .add<gameplay::CooldownCompleted>()
            .set<gameplay::MultiProj>({3, 30.f, 150.f, 30.f})
            .set<core::Speed>({150.f});

    m_world.prefab("projectile")
            .add<gameplay::Projectile>()
            .set<core::Attack>({"projectile", "enemy"})
            .set<gameplay::Chain>({
                6,
                std::unordered_set<int>()
            })
            .set<gameplay::Split>({std::unordered_set<int>()})
            .set<core::Damage>({2})
            .set<physics::Velocity2D>({0, 0})
            .set<core::DestroyAfterTime>({5})
            .set<rendering::Renderable>({
                LoadTexture("../resources/dagger.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            }).set<physics::Collider>({
                24,
                false,
                physics::CollisionFilter::player,
                physics::player_filter
            });

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

    m_world.prefab("enemy")
            .set<core::Tag>({"enemy"})
            .set<core::Health>({10, 10})
            .set<core::Damage>({1})
            .set<core::Position2D>({800, 400})
            .set<core::Speed>({25})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .add<ai::Target>(player)
            .add<ai::FollowTarget>()
            .set<ai::StoppingDistance>({16.0})
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
            .set<rendering::HealthBar>({0, 0, 50, 10});


    m_world.entity("gui_canvas").set<Rectangle>({
        0, 0, static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)
    });

    m_world.entity("enemy_spawner")
            .set<gameplay::Spawner>({"enemy"});
}


void Game::run() {
    // ON START
    m_world.progress();

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        m_world.progress(GetFrameTime());
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
