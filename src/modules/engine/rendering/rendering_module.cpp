//
// Created by laure on 1/27/2025.
//

#include "rendering_module.h"

#include "components.h"
#include "pipeline_steps.h"
#include "modules/engine/core/components.h"
#include "gui/components.h"
#include "gui/gui_module.h"
#include <raymath.h>

using namespace rendering::gui;

void rendering::RenderingModule::register_components(flecs::world world) {
    world.component<Circle>();
    world.component<Priority>();
}

void rendering::RenderingModule::register_systems(flecs::world world) {
    world.system("Before Draw")
            .kind<PreRender>()
            .run([](flecs::iter &iter) {
                BeginDrawing();
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            });

    world.system<const core::Position2D, const Circle>("Determine Visible Entities")
            .write<Visible>()
            .kind<PreRender>()
            .each([&](flecs::entity e, const core::Position2D &pos, const Circle &circle) {
                if (pos.value.x > GetScreenWidth() + circle.radius || pos.value.x < -circle.radius ||
                    pos.value.y > GetScreenHeight() + circle.radius || pos.value.y < -circle.radius) {
                    e.remove<Visible>();
                } else if (!e.has<Visible>()) {
                    e.add<Visible>();
                }
            });


    world.system<const Texture2D, const core::Position2D>("Draw Entities with Textures")
            .kind<Render>()
            .with<Visible>()
            .group_by<Priority>()
            .each([](const Texture2D &texture, const core::Position2D &position) {
                DrawTextureEx(texture, Vector2Subtract(position.value, Vector2{16, 16} / 2 * 2), 0, 2.f, WHITE);
            });

    world.system<const Circle, const core::Position2D, const Color>("Draw Entities")
            .kind<Render>()
            .with<Visible>()
            .group_by<Priority>()
            .each([](const Circle &circle, const core::Position2D &position, const Color &color) {
                DrawCircle(position.value.x, position.value.y, circle.radius, color);
            });

    world.system("After Draw")
            .kind<PostRender>()
            .run([](flecs::iter &iter) {
                EndDrawing();
            });
}

void rendering::RenderingModule::register_pipeline(flecs::world world) {
    world.component<PreRender>().add(flecs::Phase).depends_on(flecs::OnStore);
    world.component<Render>().add(flecs::Phase).depends_on<PreRender>();
    world.component<RenderGUI>().add(flecs::Phase).depends_on<Render>();
    world.component<PostRender>().add(flecs::Phase).depends_on<RenderGUI>();
}

void rendering::RenderingModule::register_submodules(flecs::world world) {
    world.import<GUIModule>();
}
