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
#include "modules/engine/rendering/gui/components.h"
#include "modules/engine/rendering/gui/gui_module.h"
#include "modules/engine/input/components.h"
#include "modules/engine/input/input_module.h"
#include "modules/engine/physics/components.h"
#include "modules/engine/physics/physics_module.h"
#include "modules/player/player_module.h"

#include "raygui.h"
#include "modules/engine/rendering/components.h"
#include "modules/engine/rendering/rendering_module.h"

Game::Game(const char *windowName, int windowWidth, int windowHeight): m_windowName(windowName),
                                                                       m_windowWidth(windowWidth),
                                                                       m_windowHeight(windowHeight),
                                                                       m_world(flecs::world()) {
    //Raylib window
    InitWindow(m_windowWidth, m_windowHeight, m_windowName.c_str());
SetTargetFPS(60);

    m_world.import<input::InputModule>();
    m_world.import<core::CoreModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();
    m_world.import<rendering::RenderingModule>();

#if not defined(EMSCRIPTEN)
    // use the flecs explorer when not on browser
    m_world.import<flecs::stats>();
    m_world.set<flecs::Rest>({});
#endif

    flecs::entity player = m_world.entity("player")
            .set<core::Position2D>({0, 0})
            .set<core::Speed>({300})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<rendering::Circle>({25})
            .set<Color>({GREEN});;

    auto hori = m_world.entity("player_horizontal_input").child_of(player)
            .set<input::InputHorizontal>({});
    m_world.entity().child_of(hori)
            .set<input::KeyBinding>({KEY_A, -1});
    m_world.entity().child_of(hori)
            .set<input::KeyBinding>({KEY_D, 1});
    m_world.entity().child_of(hori)
            .set<input::KeyBinding>({KEY_LEFT, -1});
    m_world.entity().child_of(hori)
            .set<input::KeyBinding>({KEY_RIGHT, 1});

    auto vert = m_world.entity("player_vertical_input").child_of(player)
            .set<input::InputVertical>({});
    m_world.entity().child_of(vert)
            .set<input::KeyBinding>({KEY_W, -1});
    m_world.entity().child_of(vert)
            .set<input::KeyBinding>({KEY_S, 1});
    m_world.entity().child_of(vert)
            .set<input::KeyBinding>({KEY_UP, -1});
    m_world.entity().child_of(vert)
            .set<input::KeyBinding>({KEY_DOWN, 1});

    m_world.entity("enemy")
            .set<core::Position2D>({800, 400})
            .set<core::Speed>({150})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<ai::Target>({"player"})
            .add<ai::FollowTarget>()
            .set<ai::StoppingDistance>({50.0})
            .set<rendering::Circle>({25})
            .set<Color>({RED});


    m_world.entity("button 1")
            .set<rendering::gui::Button>({
                "Hello World",
                {200, 200, 300, 100},
                {
                    m_world.system<const core::Position2D>()
                    .kind(0)
                    .each([](flecs::entity e, const core::Position2D &position) {
                        std::printf("Button Clicked\n");
                        std::printf("Entity name: %s \n", e.name().c_str());
                        std::printf("Position: (%f, %f) \n", position.value.x, position.value.y);
                    })
                }
            });

    m_world.entity("text 1")
            .set<rendering::gui::Text>({
                "Hello World",
                {m_windowWidth / 2.f, 0, 300, 100},
            });
}

void Game::run() {
    // Main game loop
    m_world.progress();
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        m_world.progress(GetFrameTime());
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
