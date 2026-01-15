//
// Created by laurent on 8/30/25.
//

#include "editor/editor_module.h"

#include <sstream>

#include "editor/components.h"
#include "editor/pipeline_steps.h"
#include "editor/systems/draw_entities_inspector_system.h"
#include "editor/systems/init_imgui_system.h"
#include "rendering/components.h"
#include "rendering/pipeline_steps.h"

void editor::EditorModule::register_components(flecs::world &world) {
    world.component<SelectedEntity>().add(flecs::Singleton);
}

void editor::EditorModule::register_systems(flecs::world &world) {
    world.system("init imgui").kind(flecs::OnStart).run(systems::init_imgui_system);

    world.system("new frame editor").kind<EditorStart>().run([](flecs::iter &it) {
        rlImGuiBegin();
        if (ImGui::BeginMainMenuBar()) {
            ImGui::MenuItem("File");
            ImGui::MenuItem("Debug");
        }
        ImGui::EndMainMenuBar();
#ifdef IMGUI_HAS_DOCK
        ImGui::DockSpaceOverViewport(
                0, NULL, ImGuiDockNodeFlags_PassthruCentralNode); // set ImGuiDockNodeFlags_PassthruCentralNode so that
                                                                  // we can see the raylib contents behind the dockspace
#endif
    });

    world.system<Console>("forward logs to editor console").kind(flecs::OnStart).each([](Console &console) {
        core::Logger::Instance().AddSink([&](core::LogLevel level, core::LogLocation loc, std::string message) {
            std::stringstream s;
            s << core::Logger::Instance().get_level_string(level) << " "
              << core::Logger::Instance().get_location_string(loc) << " " << message << std::endl;
            console.logs.push_back(s.str());
        });
    });


    world.system<rendering::VirtualViewport, Window>("draw viewport window")
            .kind<RenderEditor>()
            .each([world](flecs::entity e, rendering::VirtualViewport &viewport, Window &window) {
                if (ImGui::Begin(window.name.c_str())) {

                    auto pos = ImGui::GetCursorScreenPos();
                    viewport.rect.x = pos.x;
                    viewport.rect.y = pos.y;

                    auto content_size = ImGui::GetContentRegionAvail();
                    rlImGuiImageRenderTexture(&viewport.render_target);

                    if (IsWindowResized() || viewport.render_target.texture.width != content_size.x ||
                        viewport.render_target.texture.height != content_size.y) {
                        UnloadRenderTexture(viewport.render_target);
                        viewport.render_target = LoadRenderTexture(content_size.x, content_size.y);
                        viewport.rect.width = content_size.x;
                        viewport.rect.height = content_size.y;
                        world.event(flecs::OnSet).id<rendering::VirtualViewport>().entity(e).emit();
                    }
                }
                ImGui::End();
            });

    world.system<Window>("draw hierarchy window").with<Hierarchy>().kind<RenderEditor>().each([world](Window &window) {
        if (ImGui::Begin(window.name.c_str())) {
            if (ImGui::TreeNode("world")) {
                world.children(systems::draw_entities_inspector_system);
                ImGui::TreePop();
            }
        }
        ImGui::End();
    });

    world.system<Window, SelectedEntity>("draw inspector window")
            .with<Inspector>()
            .kind<RenderEditor>()
            .each([world](Window &window, SelectedEntity &e) {
                if (ImGui::Begin(window.name.c_str())) {
                    if (e.entity.is_alive()) {
                        //LOG_DEBUG(core::Editor, e.entity.name());
                        ImGui::Text(e.entity.name() != "" ? e.entity.name().c_str() : std::to_string(e.entity.id()).c_str());
                        ImGui::Text("Components: ");
                        e.entity.each([&](flecs::id id) {
                            if (id.is_pair()) {
                                // If id is a pair, extract & print both parts of the pair
                                flecs::entity rel = id.first();
                                flecs::entity tgt = id.second();
                                //LOG_DEBUG(core::Editor, "rel: ", rel.name(), ", ", "tgt: ", tgt.name());
                                ImGui::Text("%s", (std::string("rel: ") + std::string(rel.name()) + std::string(", ") + std::string("tgt: ") + std::string(tgt.name())).c_str());
                            } else {
                                auto comp = world.component(id);
                                //ImGuiLL
                                //std::cout << "Component: " << comp.name() << "\n";
                                ImGui::Text("%s", comp.name().c_str());

                                // // Get the type info for reflection
                                // const ecs_type_info_t *ti = ecs_get_type_info(world, id);
                                // if (!ti || !ti->component) {
                                //     std::cout << "  (no reflection data)\n";
                                //     return;
                                // }
                                //
                                // void *ptr = ecs_get_mut_id(world, e.entity, id);
                                // ecs_meta_cursor_t cur = ecs_meta_cursor(world, id, ptr);
                                //
                                // while (ecs_meta_next(&cur)) {
                                //     std::cout << "  " << ecs_meta_get_member(&cur) << " = ";
                                //
                                //     // if (ecs_meta_get_type(&cur) == EcsF32) {
                                //     //     float v;
                                //     //     ecs_meta_get_float(&cur);
                                //     //     std::cout << v;
                                //     // }
                                //     // // handle other types (int, string, nested struct, etc.)
                                //
                                //     std::cout << "\n";
                                // }
                            }
                        });
                    }
                }
                ImGui::End();
            });

    world.system<Window, Console>("draw console window")
            .kind<RenderEditor>()
            .each([world](Window &window, Console &console) {
                if (ImGui::Begin(window.name.c_str())) {
                    const auto hist = core::Logger::Instance().get_log_history();
                    for (int i = hist.size() - 1; i > 0; i--) {
                        ImGui::Text("%s", hist[i].c_str());
                    }
                }
                ImGui::End();
            });

    world.system("end frame editor").kind<EditorEnd>().run([](flecs::iter &it) { rlImGuiEnd(); });
}

void editor::EditorModule::register_entities(flecs::world &world) { world.set<SelectedEntity>({}); }
void editor::EditorModule::register_pipeline(flecs::world &world) {

    world.component<rendering::PostRender>().remove_second<rendering::RenderGUI>(flecs::DependsOn);
    world.component<EditorStart>().add(flecs::Phase).depends_on<rendering::RenderGUI>();
    world.component<RenderEditor>().add(flecs::Phase).depends_on<EditorStart>();
    world.component<EditorEnd>().add(flecs::Phase).depends_on<RenderEditor>();
    world.component<rendering::PostRender>().add(flecs::Phase).depends_on<EditorEnd>();
}
