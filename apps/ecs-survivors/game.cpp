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
#include <raylib.h>
#include <flecs.h>
#include <raygui.h>

#include "modules/ai/ai_module.h"
#include "modules/ai/components.h"
#include "core/components.h"
#include "core/core_module.h"
#include "input/components.h"
#include "input/input_module.h"
#include "physics/components.h"
#include "physics/physics_module.h"
#include "modules/player/player_module.h"
#include "raylib.h"

#include "rendering/components.h"
#include "rendering/rendering_module.h"
#include "modules/gameplay/components.h"
#include "modules/gameplay/gameplay_module.h"
#include "tilemap/tilemap_module.h"
#include "tilemap/components.h"

#include "gui/gui_module.h"
#include "gui/components.h"

#include "modules/debug/debug_module.h"

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

    SetExitKey(KEY_F4);
    init();
}

void Game::init() {

#ifndef EMSCRIPTEN
    // use the flecs explorer when not on browser
    m_world.import<flecs::stats>();
    m_world.set<flecs::Rest>({});
    m_world.set_threads(static_cast<int>(std::thread::hardware_concurrency()));
#endif

    m_world.import<core::CoreModule>();
    m_world.import<input::InputModule>();
    m_world.import<rendering::RenderingModule>();
    m_world.import<rendering::gui::GUIModule>();
    m_world.import<physics::PhysicsModule>();
    m_world.import<player::PlayerModule>();
    m_world.import<ai::AIModule>();
    m_world.import<gameplay::GameplayModule>();
    m_world.import<debug::DebugModule>();
    m_world.import<tilemap::TilemapModule>();


    m_world.set<core::GameSettings>({
        m_windowName,
        m_windowWidth,
        m_windowHeight,
        m_windowWidth,
        m_windowHeight
    });
    m_world.add<physics::CollisionRecordList>();
    m_world.set<physics::SpatialHashingGrid>({48, {0, 0}});
    m_world.set<core::Paused>({false});
    m_world.set<core::EnabledMenus>({0});
    flecs::entity player = m_world.entity("player")
            .set<core::Tag>({"player"})
            .set<core::Position2D>({2300.0f, 1300.0f})
            .set<core::Speed>({300})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({15.0})
            .set<physics::Collider>({
                true,
                false,
                {-24, -24, 48, 48},
                physics::CollisionFilter::player,
                physics::player_filter,
                physics::ColliderType::Circle,
            })
            .set<physics::CircleCollider>({24})
            .set<rendering::Priority>({2})
            .set<rendering::Renderable>({
                LoadTexture("../assets/player.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            })
            .set<gameplay::Health>({150, 150})
            .set<gameplay::RegenHealth>({1.0f})
            .set<gameplay::Experience>({1, 0, 10});

    m_world.entity("dagger attack").child_of(player)
            .add<gameplay::Projectile>()
            .set<gameplay::Attack>({"projectile", "enemy"})
            .set<gameplay::Cooldown>({1.0f, 1})
            .add<gameplay::CooldownCompleted>()
            .set<core::Speed>({150.f});

    m_world.prefab("projectile")
            .add<gameplay::Projectile>()
            .set<gameplay::Attack>({"projectile", "enemy"})
            .set<gameplay::Damage>({2})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::Collider>({
                false,
                false,
                {-18, -18, 36, 36},
                physics::CollisionFilter::player,
                physics::player_filter,
                physics::ColliderType::Circle,
            })
            .set<physics::CircleCollider>({18})
            .set<rendering::Priority>({1})
            .set<rendering::Renderable>({
                LoadTexture("../assets/dagger.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            })
            .set<core::DestroyAfterTime>({15});

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
            .set<gameplay::GiveExperience, gameplay::OnDeathEffect>({player, 2})
            .add<ai::Target>(player)
            .add<ai::FollowTarget>()
            .set<ai::StoppingDistance>({16.0})
            .set<physics::Velocity2D>({0, 0})
            .set<physics::DesiredVelocity2D>({0, 0})
            .set<physics::AccelerationSpeed>({5.0})
            .set<physics::Collider>({
                true,
                false,
                {-24, -24, 48, 48},
                physics::CollisionFilter::enemy, physics::enemy_filter,
                physics::ColliderType::Circle
            })
            .set<physics::CircleCollider>({24})
            .set<rendering::Renderable>({
                LoadTexture("../assets/ghost.png"), // 8x8
                {0, 0},
                3.f,
                WHITE
            })
            .set<rendering::Priority>({0});

    auto spawner = m_world.entity("enemy_spawner")
            .set<gameplay::Spawner>({enemy, 1});

    m_world.entity("tilemap_1")
            .set<tilemap::Tilemap>({
                "../assets/tiled/maps/sampleMap.tmx",
                3.0f
            });

    m_world.set<rendering::TrackingCamera>({
        player,
        Camera2D{0}
    });

    auto exp_panel = m_world.entity("exp_panel")
            .child_of(rendering::gui::GUIModule::gui_canvas)
            .is_a(rendering::gui::GUIModule::panel_prefab)
            .set<Rectangle>({-250, -65, 500, 60})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::BOTTOM});

    auto exp_bar = m_world.entity("exp_bar").child_of(exp_panel)
            .set<rendering::gui::ProgressBar>({0, 10, 0})
            .set<Rectangle>({-200, -30, 400, 20})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::BOTTOM});

    auto exp_level_txt = m_world.entity("exp_level_txt").child_of(exp_panel)
            .set<rendering::gui::Text>({
                "Level: 1", rendering::gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, rendering::gui::GUIModule::font_color()
            })
            .set<Rectangle>({-37.5, 10, 75, 20})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP});

    auto pause_menu = m_world.entity("pause_menu").child_of(rendering::gui::GUIModule::gui_canvas).is_a(
                rendering::gui::GUIModule::panel_prefab)
            .set<Rectangle>({-150, -200, 300, 400})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::MIDDLE})
            .add<core::PauseOnEnabled>()
            .disable();

    pause_menu.child()
            .set<Rectangle>({-150, 5, 300, 50})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP})
            .set<rendering::gui::Text>({
                "Paused", rendering::gui::FONT_SIZE_48, TEXT_ALIGN_CENTER, rendering::gui::GUIModule::font_color()
            }).disable();

    flecs::entity resume_btn = pause_menu.child().is_a(rendering::gui::GUIModule::button_prefab)
            .set<Rectangle>({-125, -135, 250, 50})
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::CENTER, rendering::gui::VERTICAL_ANCHOR::BOTTOM
            })
            .set<rendering::gui::ButtonCallback>({
                [pause_menu] {
                    pause_menu.add<core::Close>();
                }
            }).disable();

    resume_btn.get_mut<rendering::gui::Text>().text = "Resume Game";

#ifndef EMSCRIPTEN
    flecs::entity close_btn = pause_menu.child().is_a(rendering::gui::GUIModule::button_prefab)
            .set<Rectangle>({-125, -75, 250, 50})
            .set<rendering::gui::Anchor>({
                rendering::gui::HORIZONTAL_ANCHOR::CENTER, rendering::gui::VERTICAL_ANCHOR::BOTTOM
            })
            .set<rendering::gui::ButtonCallback>({
                [&] {
                    m_world.add<core::ExitConfirmed>();
                }
            }).disable();

    close_btn.get_mut<rendering::gui::Text>().text = "Close Game";
#else
    resume_btn.set<Rectangle>({-125, -75, 250, 50});
#endif


    auto input_toggle = pause_menu.child().add<input::InputToggleEnable>();
    input_toggle.child().set<input::KeyBinding>({KEY_ESCAPE, 0});

    auto level_up_menu = m_world.entity("level_up_menu").child_of(rendering::gui::GUIModule::gui_canvas).is_a(
                rendering::gui::GUIModule::panel_prefab)
            .set<Rectangle>({-300, -200, 600, 400})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::MIDDLE})
            .add<core::PauseOnEnabled>();


    m_world.entity().child_of(level_up_menu).set_name("level up menu text")
            .set<Rectangle>({-150, 5, 300, 50})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP})
            .set<rendering::gui::Text>({
                "You Leveled Up, Pick an upgrade", rendering::gui::FONT_SIZE_48, TEXT_ALIGN_CENTER,
                rendering::gui::GUIModule::font_color()
            });


    level_up_menu.disable();

    player.observe<gameplay::LevelUpEvent>([exp_bar, exp_level_txt, level_up_menu](gameplay::LevelUpEvent &event) {
        exp_bar.get_mut<rendering::gui::ProgressBar>().max_val = event.threshold;
        exp_level_txt.get_mut<rendering::gui::Text>().text = "Level: " + std::to_string(event.level);
        level_up_menu.add<core::Open>();
    });

    player.observe<gameplay::LevelUpEvent>([&, spawner](gameplay::LevelUpEvent &event) {
        spawner.get_mut<gameplay::Spawner>().difficulty = event.level;
        gameplay::spawner_interval = std::max(0.0167f, gameplay::BASE_SPAWNER_INTERVAL - 2 * (event.level / 100.f));
        gameplay::GameplayModule::m_spawner_tick.destruct();
        gameplay::GameplayModule::m_spawner_tick = m_world.timer().interval(gameplay::spawner_interval);
        gameplay::spawn_system.set_tick_source(gameplay::GameplayModule::m_spawner_tick);

    });
    //
    player.observe<gameplay::ExpGainedEvent>([exp_bar](gameplay::ExpGainedEvent &event) {
        exp_bar.get_mut<rendering::gui::ProgressBar>().current_val = event.cur;
    });

    auto container = m_world.entity().child_of(level_up_menu).set_name(
                "level up options container")
            .set<Rectangle>({-175, -150, 350, 300})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::MIDDLE})
            .set<rendering::gui::Outline>({1, GRAY, Fade(WHITE, 0)});

    container.child().is_a(rendering::gui::GUIModule::button_prefab).set_name("Option 1")
            .set<Rectangle>({-162.5, 5, 325, 40})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP})
            .set<rendering::gui::Text>({"+1 Pierce", rendering::gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
            .set<rendering::gui::ButtonCallback>({
                [level_up_menu] {
                    gameplay::add_pierce.run();
                    gameplay::add_pierce_amt.run();
                    level_up_menu.add<core::Close>();
                }
            });

    container.child().is_a(rendering::gui::GUIModule::button_prefab).set_name("Option 2")
            .set<Rectangle>({-162.5, 50, 325, 40})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP})
            .set<rendering::gui::Text>({"+1 Chain", rendering::gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
            .set<rendering::gui::ButtonCallback>({
                [level_up_menu] {
                    gameplay::add_chain.run();
                    gameplay::add_chain_amt.run();
                    level_up_menu.add<core::Close>();
                }
            });

    container.child().is_a(rendering::gui::GUIModule::button_prefab).set_name("Option 3")
            .set<Rectangle>({-162.5, 95, 325, 40})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP})
            .set<rendering::gui::Text>({"+1 Projectile", rendering::gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
            .set<rendering::gui::ButtonCallback>({
                [level_up_menu] {
                    gameplay::add_multiproj.run();
                    gameplay::add_proj.run();
                    level_up_menu.add<core::Close>();
                }
            });

    container.child().is_a(rendering::gui::GUIModule::button_prefab).set_name("Option 4")
            .set<Rectangle>({-162.5, 140, 325, 40})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP})
            .set<rendering::gui::Text>({"+1 Bounce", rendering::gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
            .set<rendering::gui::ButtonCallback>({
                [level_up_menu] {
                    gameplay::add_bounce.run();
                    gameplay::add_bounce_amt.run();
                    level_up_menu.add<core::Close>();
                }
            });

    flecs::entity split_level_up = container.child().is_a(rendering::gui::GUIModule::button_prefab).set_name("Option 5")
            .set<Rectangle>({-162.5, 185, 325, 40})
            .set<rendering::gui::Anchor>({rendering::gui::CENTER, rendering::gui::TOP})
            .set<rendering::gui::Text>({"Projectiles can split", 32, TEXT_ALIGN_CENTER, BLACK});

    split_level_up.set<rendering::gui::ButtonCallback>({
        [level_up_menu,split_level_up] {
            gameplay::add_split.run();
            level_up_menu.add<core::Close>();
            split_level_up.destruct();
        }
    });

    container.add<core::Close>();
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

void Game::UpdateDrawFrameDesktop() {
    m_world.progress(GetFrameTime());
}

void Game::UpdateDrawFrameWeb(void *game) {
    Game *instance = static_cast<Game *>(game);
    instance->m_world.progress(GetFrameTime());
}
