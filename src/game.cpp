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
#include "modules/core/core_module.h"
#include "modules/core/components.h"
#include "modules/physics/components.h"
#include "modules/physics/physics_module.h"

Game::Game(const char* windowName, int windowWidth, int windowHeight):
    m_windowName(windowName),
    m_windowWidth(windowWidth),
    m_windowHeight(windowHeight),
    m_world(flecs::world()){

    //Raylib window
    InitWindow(m_windowWidth, m_windowHeight, m_windowName.c_str());

    m_world.import<core::CoreModule>();
    m_world.import<physics::PhysicsModule>();

    m_world.entity("player")
        .set<core::Position2D>({0,0})
        .set<physics::Velocity2D>({1,1});
}

void Game::run() {


    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        m_world.progress(GetFrameTime());

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
