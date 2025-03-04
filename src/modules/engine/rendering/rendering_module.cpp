//
// Created by laure on 1/27/2025.
//

#include "rendering_module.h"

#include "components.h"
#include "pipeline_steps.h"
#include "modules/engine/core/components.h"
#include "raygui.h"
#include "gui/components.h"
#include "gui/gui_module.h"
#include "modules/engine/core/core_module.h"

using namespace rendering::gui;

void rendering::RenderingModule::register_components(flecs::world world) {
    world.component<Circle>();
}

void rendering::RenderingModule::register_systems(flecs::world world) {
    world.system("On start set anchored positions")
            .kind(flecs::OnStart)
            .run([world](flecs::iter &iter) {
                auto entity = world.lookup("gui_canvas");
                entity.children([](flecs::entity child) {
                    child.emit<core::WindowResizedEvent>();
                });
            });

    world.system("Window Resized")
            .kind<PreRender>()
            .run([world](flecs::iter &iter) {
                if (IsWindowResized()) {
                    auto entity = world.lookup("gui_canvas");
                    entity.set<Rectangle>({
                        0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
                    });

                    entity.children([](flecs::entity child) {
                        child.emit<core::WindowResizedEvent>();
                    });
                }
            });

    world.system("Before Draw")
            .kind<PreRender>()
            .run([](flecs::iter &iter) {
                BeginDrawing();
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            });

    world.system<const Circle, const core::Position2D, const Color>()
            .kind<Render>()
            .each([](const Circle &circle, const core::Position2D &position, const Color &color) {
                DrawCircle(position.value.x, position.value.y, circle.radius, color);
            });

    world.system("After Draw")
            .kind<PostRender>()
            .run([world](flecs::iter &iter) {
                EndDrawing();
            });
}

void rendering::RenderingModule::register_pipeline(flecs::world world) {
    world.component<PreRender>().add(flecs::Phase).depends_on(flecs::PostUpdate);
    world.component<Render>().add(flecs::Phase).depends_on<PreRender>();
    world.component<RenderGUI>().add(flecs::Phase).depends_on<Render>();
    world.component<PostRender>().add(flecs::Phase).depends_on<RenderGUI>();
}

void rendering::RenderingModule::register_submodules(flecs::world world) {
    world.import<GUIModule>();
}
