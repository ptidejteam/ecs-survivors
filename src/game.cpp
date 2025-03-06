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


#if not defined(EMSCRIPTEN)
    // use the flecs explorer when not on browser
    m_world.import<flecs::stats>();
    m_world.set<flecs::Rest>({});
#endif

    m_world.entity("gui_canvas").set<Rectangle>({
        0, 0, static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)
    });
    m_world.set<core::GameSettings>({m_windowName, m_windowWidth, m_windowHeight});

    flecs::entity player = m_world.entity("player")
            .set<core::Position2D>({0, 0})
            .set<core::Speed>({300})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<rendering::Circle>({25})
            .set<Color>({GREEN});

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


    m_world.entity("button 1").child_of(m_world.lookup("gui_canvas"))
            .set<rendering::gui::Button>({
                "A button",
                {
                    m_world.system<const core::Position2D>().kind(0).each(
                        [](flecs::entity e, const core::Position2D &position) {
                            std::printf("Button Clicked\n");
                            std::printf("Entity name: %s \n", e.name().c_str());
                            std::printf("Position: (%f, %f) \n", position.value.x,
                                        position.value.y);
                        })
                }
            })
            .set<Rectangle>({-150, -300, 300, 100})
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::CENTER,
                rendering::gui::VERTICAL_ANCHOR::BOTTOM
            });

    m_world.entity("text 0").child_of(m_world.lookup("gui_canvas"))
            .set<rendering::gui::Text>({"A text 0", TEXT_ALIGN_LEFT})
            .set<Rectangle>({-300, 0, 600, 300})
            .set<rendering::gui::Outline>({
                2,
                GetColor(GuiGetStyle(LABEL, BORDER_COLOR_NORMAL)),
                GetColor(GuiGetStyle(LABEL, BACKGROUND_COLOR))
            })
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::CENTER,
                rendering::gui::VERTICAL_ANCHOR::TOP
            });

    m_world.entity("text 1").child_of(m_world.lookup("gui_canvas"))
            .set<rendering::gui::Text>({"A text 1", TEXT_ALIGN_CENTER})
            .set<Rectangle>({0, 0, 200, 300})
            .set<rendering::gui::Outline>({
                2,
                GetColor(GuiGetStyle(LABEL, BORDER_COLOR_NORMAL)),
                GetColor(GuiGetStyle(LABEL, BACKGROUND_COLOR))
            })
            .set<rendering::gui::Anchor>({});

    m_world.entity("text 2").child_of(m_world.lookup("gui_canvas"))
            .set<rendering::gui::Text>({"A text 2", TEXT_ALIGN_RIGHT})
            .set<Rectangle>({-200, -300, 200, 300})
            .set<rendering::gui::Outline>({
                2,
                GetColor(GuiGetStyle(LABEL, BORDER_COLOR_NORMAL)),
                GetColor(GuiGetStyle(LABEL, BACKGROUND_COLOR))
            })
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::RIGHT,
                rendering::gui::VERTICAL_ANCHOR::BOTTOM
            });

    m_world.entity("panel").child_of(m_world.lookup("gui_canvas"))
            .set<Rectangle>({-387.5f, -150, 775, 150})
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::CENTER, rendering::gui::VERTICAL_ANCHOR::BOTTOM
            })
            .set<rendering::gui::Panel>({"A Panel"});

    for (int i = 0; i < 6; i++) {
        m_world.entity().child_of(m_world.lookup("gui_canvas::panel"))
                .set<Rectangle>({25 + i * 125.f, -40, 100, 100})
                .set<rendering::gui::Anchor>({
                    rendering::gui::HORIZONTAL_ANCHOR::LEFT,
                    rendering::gui::VERTICAL_ANCHOR::MIDDLE
                })
                .set<rendering::gui::Outline>({
                    2,
                    GetColor(GuiGetStyle(LABEL, BORDER_COLOR_NORMAL)),
                    GetColor(GuiGetStyle(LABEL, BACKGROUND_COLOR))
                });
    }
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
