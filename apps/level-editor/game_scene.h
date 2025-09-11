//
// Created by laurent on 8/30/25.
//

#ifndef TEST_GAME_SCENE_H
#define TEST_GAME_SCENE_H

#include <raygui.h>
#include <raylib.h>
#include "ai/components.h"
#include "core/components.h"
#include "core/scene.h"
#include "gameplay/components.h"
#include "gameplay/gameplay_module.h"
#include "gui/components.h"
#include "gui/gui_module.h"
#include "gui/prefabs.h"
#include "input/components.h"
#include "physics/components.h"
#include "physics/physics_module.h"
#include "rendering/components.h"
#include "rendering/rendering_module.h"
#include "tilemap/components.h"
class GameScene : public core::Scene {
public:
    ~GameScene() override = default;
    void load(flecs::world &world) override {
        flecs::entity player = world.entity("player")
                                       .child_of(rendering::RenderingModule::main_viewport)
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
                                       .set<rendering::Renderable>({LoadTexture("../../assets/player.png"), // 8x8
                                                                    {0, 0},
                                                                    3.f,
                                                                    WHITE})
                                       .set<gameplay::Health>({150, 150})
                                       .set<gameplay::RegenHealth>({1.0f})
                                       .set<gameplay::Experience>({1, 0, 2});

        world.entity("dagger attack")
                .child_of(player)
                .add<gameplay::Projectile>()
                .set<gameplay::Attack>({"projectile", "enemy"})
                .set<gameplay::Cooldown>({1.0f, 1})
                .add<gameplay::CooldownCompleted>()
                .set<core::Speed>({150.f});

        world.prefab("projectile")
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
                .set<rendering::Renderable>({LoadTexture("../../assets/dagger.png"), // 8x8
                                             {0, 0},
                                             3.f,
                                             WHITE})
                .set<core::DestroyAfterTime>({15});

        auto hori = world.entity("player_horizontal_input").child_of(player).set<input::InputHorizontal>({});
        world.entity().child_of(hori).set<input::KeyBinding>({KEY_A, -1});
        world.entity().child_of(hori).set<input::KeyBinding>({KEY_D, 1});
        world.entity().child_of(hori).set<input::KeyBinding>({KEY_LEFT, -1});
        world.entity().child_of(hori).set<input::KeyBinding>({KEY_RIGHT, 1});

        auto vert = world.entity("player_vertical_input").child_of(player).set<input::InputVertical>({});
        world.entity().child_of(vert).set<input::KeyBinding>({KEY_W, -1});
        world.entity().child_of(vert).set<input::KeyBinding>({KEY_S, 1});
        world.entity().child_of(vert).set<input::KeyBinding>({KEY_UP, -1});
        world.entity().child_of(vert).set<input::KeyBinding>({KEY_DOWN, 1});

        flecs::entity enemy = world.prefab("enemy")
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
                                      .set<physics::Collider>({true,
                                                               false,
                                                               {-24, -24, 48, 48},
                                                               physics::CollisionFilter::enemy,
                                                               physics::enemy_filter,
                                                               physics::ColliderType::Circle})
                                      .set<physics::CircleCollider>({24})
                                      .set<rendering::Renderable>({LoadTexture("../../assets/ghost.png"), // 8x8
                                                                   {0, 0},
                                                                   3.f,
                                                                   WHITE})
                                      .set<rendering::Priority>({0});

        auto spawner = world.entity("enemy_spawner")
                               .child_of(rendering::RenderingModule::main_viewport)
                               .set<gameplay::Spawner>({enemy, 1});

        world.entity("tilemap_1").set<tilemap::Tilemap>({"../../assets/tiled/maps/sampleMap.tmx", 3.0f});

        world.set<rendering::TrackingCamera>({player, Camera2D{0}});

        auto exp_panel = world.entity("exp_panel")
                                 .child_of(gui::GUIModule::gui_canvas)
                                 .is_a<gui::PanelPrefab>()
                                 .set<Rectangle>({-250, -65, 500, 60})
                                 .set<gui::Anchor>({gui::CENTER, gui::BOTTOM});

        auto exp_bar = world.entity("exp_bar")
                               .child_of(exp_panel)
                               .is_a<gui::ProgressBarPrefab>()
                               .set<gui::ProgressBar>({0, 10, 0})
                               .set<Rectangle>({-200, -30, 400, 20})
                               .set<gui::Anchor>({gui::CENTER, gui::BOTTOM});

        auto exp_level_txt = world.entity("exp_level_txt")
                                     .child_of(exp_panel)
                                     .is_a<gui::TextPrefab>()
                                     .set<gui::Text>({"Level: 1", gui::FONT_SIZE_32, TEXT_ALIGN_CENTER,
                                                      gui::GUIModule::font_color()})
                                     .set<Rectangle>({-37.5, 10, 75, 20})
                                     .set<gui::Anchor>({gui::CENTER, gui::TOP});

        auto pause_menu = world.entity("pause_menu")
                                  .child_of(gui::GUIModule::gui_canvas)
                                  .is_a<gui::PanelPrefab>()
                                  .set<Rectangle>({-150, -200, 300, 400})
                                  .set<gui::Anchor>({gui::CENTER, gui::MIDDLE})
                                  .add<core::PauseOnEnabled>()
                                  .disable();

        pause_menu.child()
                .is_a<gui::TextPrefab>()
                .set<Rectangle>({-150, 5, 300, 50})
                .set<gui::Anchor>({gui::CENTER, gui::TOP})
                .set<gui::Text>({"Paused", gui::FONT_SIZE_48, TEXT_ALIGN_CENTER, gui::GUIModule::font_color()})
                .disable();

        flecs::entity resume_btn =
                pause_menu.child()
                        .set_name("resume_btn")
                        .is_a<gui::ButtonPrefab>()
                        .set<Rectangle>({-125, -135, 250, 50})
                        .set<gui::Anchor>({gui::HORIZONTAL_ANCHOR::CENTER, gui::VERTICAL_ANCHOR::BOTTOM})
                        .set<gui::ButtonCallback>({[pause_menu] { pause_menu.add<core::Close>(); }})
                        .disable();

        resume_btn.child()
                .set_name("resume_btn_text")
                .is_a<gui::TextPrefab>()
                .set<gui::Text>({"Resume Game", gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
                .set<Rectangle>({0, 0, 250, 50})
                .set<gui::InteractableElement>({BLACK, WHITE, DARKGRAY})
                .add<gui::InteractableElementState>(gui::InteractableElementState::Normal);

#ifndef EMSCRIPTEN
        flecs::entity close_btn =
                pause_menu.child()
                        .is_a<gui::ButtonPrefab>()
                        .set<Rectangle>({-125, -75, 250, 50})
                        .set<gui::Anchor>({gui::HORIZONTAL_ANCHOR::CENTER, gui::VERTICAL_ANCHOR::BOTTOM})
                        .set<gui::ButtonCallback>({[&] { world.add<core::ExitConfirmed>(); }})
                        .disable();

        close_btn.child()
                .is_a<gui::TextPrefab>()
                .set<gui::Text>({"Exit Game", gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
                .set<Rectangle>({0, 0, 250, 50})
                .set<gui::InteractableElement>({BLACK, WHITE, DARKGRAY})
                .add<gui::InteractableElementState>(gui::InteractableElementState::Normal);
#else
        resume_btn.set<Rectangle>({-125, -75, 250, 50});
#endif


        auto input_toggle = pause_menu.child().add<input::InputToggleEnable>();
        input_toggle.child().set<input::KeyBinding>({KEY_ESCAPE, 0});

        auto level_up_menu = world.entity("level_up_menu")
                                     .child_of(gui::GUIModule::gui_canvas)
                                     .is_a<gui::PanelPrefab>()
                                     .set<Rectangle>({-300, -200, 600, 400})
                                     .set<gui::Anchor>({gui::CENTER, gui::MIDDLE})
                                     .add<core::PauseOnEnabled>();


        world.entity()
                .child_of(level_up_menu)
                .set_name("level up menu text")
                .is_a<gui::PanelPrefab>()
                .set<Rectangle>({-150, 5, 300, 50})
                .set<gui::Anchor>({gui::CENTER, gui::TOP})
                .set<gui::Text>({"You Leveled Up, Pick an upgrade", gui::FONT_SIZE_48, TEXT_ALIGN_CENTER,
                                 gui::GUIModule::font_color()});


        level_up_menu.disable();

        player.observe<gameplay::LevelUpEvent>(
                [&, exp_bar, exp_level_txt, level_up_menu](gameplay::LevelUpEvent &event) {
                    exp_bar.get_mut<gui::ProgressBar>().max_val = event.threshold;
                    exp_level_txt.get_mut<gui::Text>().text = "Level: " + std::to_string(event.level);
                    level_up_menu.add<core::Open>();
                });

        player.observe<gameplay::LevelUpEvent>([&, spawner](gameplay::LevelUpEvent &event) {
            spawner.get_mut<gameplay::Spawner>().difficulty = event.level;
            gameplay::spawner_interval = std::max(0.0167f, gameplay::BASE_SPAWNER_INTERVAL - 2 * (event.level / 100.f));
            gameplay::GameplayModule::m_spawner_tick.destruct();
            gameplay::GameplayModule::m_spawner_tick = world.timer().interval(gameplay::spawner_interval);
            gameplay::spawn_system.set_tick_source(gameplay::GameplayModule::m_spawner_tick);
        });

        player.observe<gameplay::ExpGainedEvent>([exp_bar](gameplay::ExpGainedEvent &event) {
            exp_bar.get_mut<gui::ProgressBar>().current_val = event.cur;
        });

        auto container = world.entity()
                                 .child_of(level_up_menu)
                                 .set_name("level up options container")
                                 .set<Rectangle>({-175, -150, 350, 300})
                                 .set<gui::Anchor>({gui::CENTER, gui::MIDDLE})
                                 .set<gui::Outline>({1, GRAY, Fade(WHITE, 0)});

        auto o1 = container.child()
                          .is_a<gui::ButtonPrefab>()
                          .set_alias("Option 1")
                          .set<Rectangle>({-162.5, 5, 325, 40})
                          .set<gui::Anchor>({gui::CENTER, gui::TOP})
                          .set<gui::ButtonCallback>({[level_up_menu] {
                              gameplay::add_pierce.run();
                              gameplay::add_pierce_amt.run();
                              level_up_menu.add<core::Close>();
                          }});
        o1.child()
                .is_a<gui::TextPrefab>()
                .set<gui::Text>({"+1 Pierce", gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
                .set<Rectangle>({0, 0, 325, 40})
                .set<gui::InteractableElement>({BLACK, WHITE, DARKGRAY})
                .add<gui::InteractableElementState>(gui::InteractableElementState::Normal);

        auto o2 = container.child()
                          .is_a<gui::ButtonPrefab>()
                          .set_name("Option 2")
                          .set<Rectangle>({-162.5, 50, 325, 40})
                          .set<gui::Anchor>({gui::CENTER, gui::TOP})
                          .set<gui::ButtonCallback>({[level_up_menu] {
                              gameplay::add_chain.run();
                              gameplay::add_chain_amt.run();
                              level_up_menu.add<core::Close>();
                          }});

        o2.child()
                .is_a<gui::TextPrefab>()
                .set<gui::Text>({"+1 Chain", gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
                .set<Rectangle>({0, 0, 325, 40})
                .set<gui::InteractableElement>({BLACK, WHITE, DARKGRAY})
                .add<gui::InteractableElementState>(gui::InteractableElementState::Normal);

        auto o3 = container.child()
                          .is_a<gui::ButtonPrefab>()
                          .set_name("Option 3")
                          .set<Rectangle>({-162.5, 95, 325, 40})
                          .set<gui::Anchor>({gui::CENTER, gui::TOP})
                          .set<gui::ButtonCallback>({[level_up_menu] {
                              gameplay::add_multiproj.run();
                              gameplay::add_proj.run();
                              level_up_menu.add<core::Close>();
                          }});
        o3.child()
                .is_a<gui::TextPrefab>()
                .set<gui::Text>({"+1 Projectile", gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
                .set<Rectangle>({0, 0, 325, 40})
                .set<gui::InteractableElement>({BLACK, WHITE, DARKGRAY})
                .add<gui::InteractableElementState>(gui::InteractableElementState::Normal);

        auto o4 = container.child()
                          .is_a<gui::ButtonPrefab>()
                          .set_name("Option 4")
                          .set<Rectangle>({-162.5, 140, 325, 40})
                          .set<gui::Anchor>({gui::CENTER, gui::TOP})
                          .set<gui::ButtonCallback>({[level_up_menu] {
                              gameplay::add_bounce.run();
                              gameplay::add_bounce_amt.run();
                              level_up_menu.add<core::Close>();
                          }});

        o4.child()
                .is_a<gui::TextPrefab>()
                .set<gui::Text>({"+1 Bounce", gui::FONT_SIZE_32, TEXT_ALIGN_CENTER, BLACK})
                .set<Rectangle>({0, 0, 325, 40})
                .set<gui::InteractableElement>({BLACK, WHITE, DARKGRAY})
                .add<gui::InteractableElementState>(gui::InteractableElementState::Normal);

        flecs::entity split_level_up = container.child()
                                               .is_a<gui::ButtonPrefab>()
                                               .set_name("Option 5")
                                               .set<Rectangle>({-162.5, 185, 325, 40})
                                               .set<gui::Anchor>({gui::CENTER, gui::TOP});

        split_level_up.set<gui::ButtonCallback>({[level_up_menu, split_level_up] {
            gameplay::add_split.run();
            level_up_menu.add<core::Close>();
            split_level_up.destruct();
        }});

        split_level_up.child()
                .is_a<gui::TextPrefab>()
                .set<gui::Text>({"Projectiles can split", 32, TEXT_ALIGN_CENTER, BLACK})
                .set<Rectangle>({0, 0, 325, 40})
                .set<gui::InteractableElement>({BLACK, WHITE, DARKGRAY})
                .add<gui::InteractableElementState>(gui::InteractableElementState::Normal);


        container.add<core::Close>();
    }
};
#endif // TEST_GAME_SCENE_H
