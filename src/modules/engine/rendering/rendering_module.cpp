//
// Created by laure on 1/27/2025.
//

#include "rendering_module.h"

#include "components.h"
#include "pipeline_steps.h"
#include "modules/engine/core/components.h"
#include "raygui.h"

void rendering::RenderingModule::register_components(flecs::world world) {
}

void rendering::RenderingModule::register_systems(flecs::world world) {
    world.system<const Circle, const core::Position2D, const Color>()
            .kind<Render>()
            .each([](const Circle &circle, const core::Position2D &position, const Color &color) {
                DrawCircle(position.value.x, position.value.y, circle.radius, color);
            });

    world.system("Before Draw")
            .kind<PreRender>()
            .run([](flecs::iter &iter) {
                BeginDrawing();
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            });

    world.system("After Draw")
            .kind<PostRender>()
            .run([](flecs::iter &iter) {
                EndDrawing();
            });
}

void rendering::RenderingModule::register_pipeline(flecs::world world) {
    world.component<PreRender>().add(flecs::Phase).depends_on(flecs::PostUpdate);
    world.component<Render>().add(flecs::Phase).depends_on(flecs::PostUpdate);
    world.component<RenderGUI>().add(flecs::Phase).depends_on(flecs::PostUpdate);
    world.component<PostRender>().add(flecs::Phase).depends_on(flecs::PostUpdate);
}

void rendering::RenderingModule::register_submodules(flecs::world world) {
    world.import<gui::GUIModule>();
}
