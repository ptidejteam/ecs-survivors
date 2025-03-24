//
// Created by Laurent Voisard on 12/20/2024.
//

#include "game.h"
#include <iostream>
#include <ostream>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

#include <filesystem>
#include <fstream>

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
#include "modules/analytics/analytics_module.h"
#include "modules/gameplay/components.h"
#include "modules/gameplay/gameplay_module.h"

Game::Game(const char *windowName, int windowWidth, int windowHeight) : m_windowName(windowName),
                                                                        m_windowWidth(windowWidth),
                                                                        m_windowHeight(windowHeight),
                                                                        m_world(flecs::world()) {
    // Raylib window
#ifndef EMSCRIPTEN
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
#endif

    InitWindow(m_windowWidth, m_windowHeight, m_windowName.c_str());
    SetTargetFPS(1000);

    //m_world.set_threads(4);
    m_world.import<core::CoreModule>();
    m_world.import<input::InputModule>();
    m_world.import<rendering::RenderingModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();
    m_world.import<gameplay::GameplayModule>();
    m_world.import<analytics::AnalyticsModule>();


#ifndef EMSCRIPTEN
    // use the flecs explorer when not on browser
    //m_world.import<flecs::stats>();
    //m_world.set<flecs::Rest>({});
#endif


    m_world.set<core::GameSettings>({m_windowName, m_windowWidth, m_windowHeight});

    flecs::entity player = m_world.entity("player")
            .set<core::Position2D>({960, 540})
            .set<core::Speed>({1000})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<rendering::Circle>({8})
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
            .set<ai::StoppingDistance>({16.0})
            .set<rendering::Circle>({8})
            .set<Texture2D>({LoadTexture("../resources/ghost.png")});

    m_world.entity("gui_canvas").set<Rectangle>({
        0, 0, static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)
    });

    m_world.entity("panel").child_of(m_world.lookup("gui_canvas"))
            .set<Rectangle>({0, -25, 100, 65})
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
            })
            .set<rendering::gui::Panel>({"A Panel"});

    // m_world.entity("naive_ecs_add_relationship_button").child_of(m_world.lookup("gui_canvas::panel"))
    //         .set<rendering::gui::Anchor>({
    //             rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
    //         })
    //         .set<Rectangle>({25, 100, 250, 100})
    //         .set<rendering::gui::Button>({
    //             "Enable Naive ECS Collision Systems (Relationships)",
    //             m_world.system().kind(0).run([&](flecs::iter &it) {
    //                 physics::PhysicsModule::change_collision_strategy(COLLISION_RELATIONSHIP);
    //             })
    //         });
    //
    // m_world.entity("naive_ecs_add_entity_button").child_of(m_world.lookup("gui_canvas::panel"))
    //         .set<rendering::gui::Anchor>({
    //             rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
    //         })
    //         .set<Rectangle>({25, 225, 250, 100})
    //         .set<rendering::gui::Button>({
    //             "Enable Naive ECS Collision Systems (Entities)",
    //             m_world.system().kind(0).run([&](flecs::iter &it) {
    //                 physics::PhysicsModule::change_collision_strategy(COLLISION_ENTITY);
    //             })
    //         });
    //
    // m_world.entity("non_ecs_accelerated_updating_button").child_of(m_world.lookup("gui_canvas::panel"))
    //         .set<rendering::gui::Anchor>({
    //             rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
    //         })
    //         .set<Rectangle>({25, 350, 250, 100})
    //         .set<rendering::gui::Button>({
    //             "Enable External Collision Systems (UPDATING)",
    //             m_world.system().kind(0).run([&](flecs::iter &it) {
    //                 physics::PhysicsModule::change_collision_strategy(SPATIAL_HASH_UPDATING);
    //             })
    //         });
    //
    // m_world.entity("non_ecs_accelerated_rebuilding_button").child_of(m_world.lookup("gui_canvas::panel"))
    //        .set<rendering::gui::Anchor>({
    //            rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
    //        })
    //        .set<Rectangle>({25, 475, 250, 100})
    //        .set<rendering::gui::Button>({
    //            "Enable External Collision Systems (REBUILDING)",
    //            m_world.system().kind(0).run([&](flecs::iter &it) {
    //                physics::PhysicsModule::change_collision_strategy(SPATIAL_HASH_REBUILDING);
    //            })
    //        });
    // m_world.entity("box2d_button").child_of(m_world.lookup("gui_canvas::panel"))
    //        .set<rendering::gui::Anchor>({
    //            rendering::gui::HORIZONTAL_ANCHOR::LEFT, rendering::gui::VERTICAL_ANCHOR::TOP
    //        })
    //        .set<Rectangle>({25, 600, 250, 100})
    //        .set<rendering::gui::Button>({
    //            "Enable Box2d Collisions (REBUILDING)",
    //            m_world.system().kind(0).run([&](flecs::iter &it) {
    //                physics::PhysicsModule::change_collision_strategy(BOX2D);
    //            })
    //        });

    m_world.entity("event_bus").add<EventBus>();

    m_world.entity("enemy_spawner")
            .set<gameplay::Spawner>({"enemy"});
}


void Game::run() {
    // ON START

    physics::PhysicsModule::change_collision_strategy(m_strategy);
    m_world.progress();

    SetWindowFocused();
    std::vector<std::chrono::microseconds> delta_times;
    std::vector<int> frameRates;
    std::vector<int> entities;
    int frames = 0;
    // Main game loop
    auto q = m_world.query<core::Position2D>();

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        auto start = std::chrono::high_resolution_clock::now();
        m_world.progress(0.0167f);
        auto end = std::chrono::high_resolution_clock::now();
        delta_times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start));
        entities.push_back(q.count());
        frames++;


        auto time = 0L;
        int count = 0;
        if(frames < 2) {
            frameRates.push_back(0);
            continue;
        }
        for (int i = 1; i < frames; i++) {
            time += delta_times[frames - i].count();
            count++;
            if (time >= 1000000)
                break;
        }

        frameRates.push_back(count / (time / 1000000.0f));

        if (time >= 1000000 && count <= 30) {
            std::cout << time << std::endl;
            std::cout << count << std::endl;
            std::string target = std::format("results/{}/{}.png", m_windowName.substr(0, m_windowName.size() - 2), m_windowName);
            std::string name = std::format("{}.png", m_windowName);
            TakeScreenshot(name.c_str());
            try {
                if (std::filesystem::exists(target))
                    std::filesystem::remove(target);
                std::filesystem::copy(name, target);
                std::filesystem::remove(name);
            } catch (std::exception& e) {
                std::cout << "couldn't write png" << e.what() << std::endl;
            }
            break;
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    try {
        //--------------------------------------------------------------------------------------
        if (std::ofstream file(std::format("./results/{}/{}.txt", m_windowName.substr(0, m_windowName.size() - 2), m_windowName)); file.is_open()) {
            for (int i = 0; i < frames; i++) {
                file << i << "," << entities[i] << "," << frameRates[i] << "," << delta_times[i].count() << "\n";
            }
            file.close();
        } else {
            printf("Failed to open file %s\n",
                   std::format("./results/{}/{}.txt", m_windowName.substr(0, m_windowName.size() - 2), m_windowName).c_str());
        }
    }
    catch(std::exception& e) {
        std::cout << "could not write results" << e.what() << std::endl;
    }
}
