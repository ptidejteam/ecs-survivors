//
// Created by laurent on 8/30/25.
//

#include "editor/editor_module.h"

#include "editor/components.h"
#include "editor/pipeline_steps.h"
#include "editor/systems/init_imgui_system.h"
#include "rendering/components.h"
#include "rendering/pipeline_steps.h"

void editor::EditorModule::register_components(flecs::world &world) {}

void editor::EditorModule::register_systems(flecs::world &world) {
    world.system("init imgui").kind(flecs::OnStart).run(systems::init_imgui_system);

    world.system("new frame editor").kind<EditorStart>().run([] (flecs::iter& it) {
        rlImGuiBegin();
#ifdef IMGUI_HAS_DOCK
		ImGui::DockSpaceOverViewport(0,  NULL, ImGuiDockNodeFlags_PassthruCentralNode); // set ImGuiDockNodeFlags_PassthruCentralNode so that we can see the raylib contents behind the dockspace
#endif
    });

    world.system<rendering::Viewport, Window>("draw viewport window")
            .kind<RenderEditor>()
            .each([world](flecs::entity e, rendering::Viewport &viewport, Window &window) {
                if (ImGui::Begin(window.name.c_str())) {

                    auto pos = ImGui::GetCursorScreenPos();
                    viewport.rect.x = pos.x;
                    viewport.rect.y = pos.y;

                    auto a = ImGui::GetContentRegionAvail();
                    if (viewport.render_target.texture.width != a.x || viewport.render_target.texture.height != a.y) {
                        UnloadRenderTexture(viewport.render_target);
                        viewport.render_target = LoadRenderTexture(a.x, a.y);
                        world.event(flecs::OnSet).id<rendering::Viewport>().entity(e).emit();
                        world.get_mut<rendering::Settings>().window_width = a.x;
                        world.get_mut<rendering::Settings>().window_height = a.y;
                    }

                    rlImGuiImageRenderTexture(&viewport.render_target);
                }
                ImGui::End();
            });

    world.system<Window>("draw inspector window")
            .with<Inspector>()
            .kind<RenderEditor>()
            .each([](Window &window) {
                if (ImGui::Begin(window.name.c_str())) {
                }
                ImGui::End();
            });

    world.system("end frame editor").kind<EditorEnd>().run([] (flecs::iter& it) {
        rlImGuiEnd();
    });
}

void editor::EditorModule::register_entities(flecs::world &world) {}
void editor::EditorModule::register_pipeline(flecs::world &world) {

    world.component<rendering::PostRender>().remove_second<rendering::RenderGUI>(flecs::DependsOn);
    world.component<EditorStart>().add(flecs::Phase).depends_on<rendering::RenderGUI>();
    world.component<RenderEditor>().add(flecs::Phase).depends_on<EditorStart>();
    world.component<EditorEnd>().add(flecs::Phase).depends_on<RenderEditor>();
    world.component<rendering::PostRender>().add(flecs::Phase).depends_on<EditorEnd>();
}
