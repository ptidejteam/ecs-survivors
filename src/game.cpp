//
// Created by Laurent Voisard on 12/20/2024.
//

#include "game.h"
#include <iostream>
#include <ostream>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

#include <raygui.h>


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

#include "modules/engine/rendering/components.h"
#include "modules/engine/rendering/rendering_module.h"
#include "raygui.h"
#include "modules/engine/rendering/gui/gui_module.h"
#include "modules/gameplay/components.h"
#include "modules/gameplay/gameplay_module.h"

Game::Game(const char *windowName, int windowWidth, int windowHeight) : m_windowName(windowName),
                                                                        m_windowWidth(windowWidth),
                                                                        m_windowHeight(windowHeight),
                                                                        m_world(flecs::world()) {
    // Raylib window
#ifndef EMSCRIPTEN
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
#endif

    InitWindow(m_windowWidth, m_windowHeight, m_windowName.c_str());
    SetTargetFPS(60);


    m_world.import<input::InputModule>();
    m_world.import<core::CoreModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();
    m_world.import<rendering::RenderingModule>();
    m_world.import<gameplay::GameplayModule>();


#if not defined(EMSCRIPTEN)
    // use the flecs explorer when not on browser
    m_world.import<flecs::stats>();
    m_world.set<flecs::Rest>({});
#endif


    m_world.set<core::GameSettings>({m_windowName, m_windowWidth, m_windowHeight});

    flecs::entity player = m_world.entity("player")
            .set<core::Position2D>({800, 400})
            .set<core::Speed>({1000})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<rendering::Circle>({25})
            .set<Color>({GREEN})
            .add<rendering::Priority>(1);


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

    m_world.entity("collision_records_container");

    m_world.prefab("enemy")
            .set<core::Position2D>({800, 400})
            .set<core::Speed>({25})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<ai::Target>({"player"})
            .add<ai::FollowTarget>()
            .set<ai::StoppingDistance>({50.0})
            .set<rendering::Circle>({16})
            .set<Texture2D>({LoadTexture("../resources/ghost.png")});

    m_world.entity("gui_canvas").set<Rectangle>({
        0, 0, static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)
    });

    m_world.entity("panel").child_of(m_world.lookup("gui_canvas"))
            .set<Rectangle>({0, -25, 300, 475})
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
            })
            .set<rendering::gui::Panel>({"A Panel"});

    m_world.entity("naive_ecs_add_relationship_button").child_of(m_world.lookup("gui_canvas::panel"))
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
            })
            .set<Rectangle>({25, 100, 250, 100})
            .set<rendering::gui::Button>({
                "Enable Naive ECS Collision Systems (Relationships)",
                m_world.system().kind(0).run([&](flecs::iter &it) {
                    physics::PhysicsModule::s1.enable();
                    physics::PhysicsModule::s2.enable();

                    physics::PhysicsModule::s3.disable();
                    physics::PhysicsModule::s4.disable();
                    physics::PhysicsModule::s5.disable();

                    physics::PhysicsModule::s6.disable();
                    physics::PhysicsModule::s7.disable();
                    physics::PhysicsModule::s8.disable();
                    physics::PhysicsModule::s9.disable();
                })
            });

    m_world.entity("naive_ecs_add_entity_button").child_of(m_world.lookup("gui_canvas::panel"))
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
            })
            .set<Rectangle>({25, 225, 250, 100})
            .set<rendering::gui::Button>({
                "Enable Naive ECS Collision Systems (Entities)",
                m_world.system().kind(0).run([&](flecs::iter &it) {
                    physics::PhysicsModule::s1.disable();
                    physics::PhysicsModule::s2.disable();

                    physics::PhysicsModule::s3.enable();
                    physics::PhysicsModule::s4.enable();
                    physics::PhysicsModule::s5.enable();

                    physics::PhysicsModule::s6.disable();
                    physics::PhysicsModule::s7.disable();
                    physics::PhysicsModule::s8.disable();
                    physics::PhysicsModule::s9.disable();
                })
            });

    m_world.entity("non_ecs_accelerated_button").child_of(m_world.lookup("gui_canvas::panel"))
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
            })
            .set<Rectangle>({25, 350, 250, 100})
            .set<rendering::gui::Button>({
                "Enable External Collision Systems (Entities)",
                m_world.system().kind(0).run([&](flecs::iter &it) {
                    physics::PhysicsModule::s1.disable();
                    physics::PhysicsModule::s2.disable();

                    physics::PhysicsModule::s3.disable();
                    physics::PhysicsModule::s4.disable();
                    physics::PhysicsModule::s5.disable();

                    physics::PhysicsModule::s6.enable();
                    physics::PhysicsModule::s7.enable();
                    physics::PhysicsModule::s8.enable();
                    physics::PhysicsModule::s9.enable();
                })
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
