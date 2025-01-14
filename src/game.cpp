//
// Created by Laurent Voisard on 12/20/2024.
//

#include "game.h"
#include <iostream>
#include <ostream>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include "modules/ai/ai_module.h"
#include "modules/ai/components.h"
#include "modules/engine/core/core_module.h"
#include "modules/engine/core/components.h"
#include "modules/engine/input/components.h"
#include "modules/engine/input/input_module.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/physics/physics_module.h"
#include "modules/player/components.h"
#include "modules/player/player_module.h"

Game::Game(const char *windowName, int windowWidth, int windowHeight): m_windowName(windowName),
                                                                       m_windowWidth(windowWidth),
                                                                       m_windowHeight(windowHeight),
                                                                       m_world(flecs::world()) {
    //Raylib window
    InitWindow(m_windowWidth, m_windowHeight, m_windowName.c_str());

    m_world.import<input::InputModule>();
    m_world.import<core::CoreModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();

#if not defined(EMSCRIPTEN)
    // use the flecs explorer when not on browser
    m_world.import<flecs::stats>();
    m_world.set<flecs::Rest>({});
#endif

    flecs::entity keybind_horizontal1 = m_world.entity("keybind_horizontal1").set<input::KeyBinding>({KEY_A, -1});
    flecs::entity keybind_horizontal2 = m_world.entity("keybind_horizontal2").set<input::KeyBinding>({KEY_D, 1});
    flecs::entity keybind_horizontal3 = m_world.entity("keybind_horizontal3").set<input::KeyBinding>({KEY_LEFT, -1});
    flecs::entity keybind_horizontal4 = m_world.entity("keybind_horizontal4").set<input::KeyBinding>({KEY_RIGHT, 1});
    flecs::entity keybind_vertical1 = m_world.entity("keybind_vertical1").set<input::KeyBinding>({KEY_W, -1});
    flecs::entity keybind_vertical2 = m_world.entity("keybind_vertical2").set<input::KeyBinding>({KEY_S, 1});
    flecs::entity keybind_vertical3 = m_world.entity("keybind_vertical3").set<input::KeyBinding>({KEY_UP, -1});
    flecs::entity keybind_vertical4 = m_world.entity("keybind_vertical4").set<input::KeyBinding>({KEY_DOWN, 1});

    flecs::entity player = m_world.entity("player")
            .set<core::Position2D>({0, 0})
            .set<core::Speed>({300})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0,0})
            .set<physics::AccelerationSpeed>({5.0})
            .add<player::PlayerControlled>()
            .add<player::MovementInputDirection>();

    m_world.entity("player_horizontal_input").child_of(player)
            .set<input::InputHorizontal>({})
            .add<input::BindingSlot1>(keybind_horizontal1)
            .add<input::BindingSlot2>(keybind_horizontal2)
            .add<input::BindingSlot3>(keybind_horizontal3)
            .add<input::BindingSlot4>(keybind_horizontal4);

    m_world.entity("player_vertical_input").child_of(player)
            .set<input::InputVertical>({})
            .add<input::BindingSlot1>(keybind_vertical1)
            .add<input::BindingSlot2>(keybind_vertical2)
            .add<input::BindingSlot3>(keybind_vertical3)
            .add<input::BindingSlot4>(keybind_vertical4);


    m_world.entity("enemy")
            .set<core::Position2D>({800, 400})
            .set<core::Speed>({150})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0,0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<ai::Target>({
                "player"
            })
            .add<ai::FollowTarget>()
            .set<ai::StoppingDistance>({
                50.0
            });
}

void Game::run() {
    SetTargetFPS(100); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawCircle(
            m_world.entity("enemy").get<core::Position2D>()->value.x,
            m_world.entity("enemy").get<core::Position2D>()->value.y,
            25.0,
            RED);
        DrawCircle(
            m_world.entity("player").get<core::Position2D>()->value.x,
            m_world.entity("player").get<core::Position2D>()->value.y,
            25.0,
            GREEN);

        m_world.progress(GetFrameTime());
        DrawFPS(10, 10);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
