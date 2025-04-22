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

#include <rlgl.h>
#include "modules/engine/physics/components.h"


void rendering::RenderingModule::register_components(flecs::world world) {
    world.component<Circle>();
    world.component<Priority>();
}

void rendering::RenderingModule::register_systems(flecs::world world) {

    world.system("Before Draw")
            .kind<PreRender>()
            .run([&, world](flecs::iter &iter) {
                BeginDrawing();
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            });

    world.system<const core::Position2D, const Renderable, const core::GameSettings>("Determine Visible Entities")
            .term_at(2).singleton()
            .write<Visible>()
            .kind<PreRender>()
            .each([&](flecs::entity e, const core::Position2D &pos, const Renderable &renderable,
                      const core::GameSettings &settings) {
                if (pos.value.x > settings.windowWidth + renderable.texture.width || pos.value.x < -renderable.texture.
                    width ||
                    pos.value.y > settings.windowHeight + renderable.texture.height || pos.value.y < -renderable.texture
                    .height) {
                    e.remove<Visible>();
                } else if (!e.has<Visible>()) {
                    e.add<Visible>();
                }
            });

    world.system<const Renderable, const core::Position2D, const Rotation *>("Draw Entities with Textures")
            .kind<Render>()
            .with<Visible>()
            .group_by<Priority>()
            .each([&](const Renderable &renderable, const core::Position2D &position, const Rotation *rotation) {
                Rectangle rec{
                    0.0f, 0.0f,
                    (float) renderable.texture.width,
                    (float) renderable.texture.height
                };

                float scaledWidth = renderable.texture.width * renderable.scale;
                float scaledHeight = renderable.texture.height * renderable.scale;

                Rectangle source{
                    position.value.x + renderable.draw_offset.x * renderable.scale,
                    position.value.y + renderable.draw_offset.y * renderable.scale,
                    scaledWidth,
                    scaledHeight
                };

                Vector2 origin = Vector2{
                    scaledWidth / 2.0f,
                    scaledHeight / 2.0f
                };

                float r = rotation ? rotation->angle : 0.0f;
                DrawTexturePro(renderable.texture, rec, source, origin, r, renderable.tint);
            });
    world.system<const HealthBar, core::Health, const core::Position2D, const Renderable>("show healthbar")
            .kind<Render>()
            .each([](flecs::entity e, const HealthBar bar, core::Health &health, const core::Position2D &pos,
                     const Renderable &renderable) {
                if (health.max - health.value <= 0.05f) return;
                GuiProgressBar(
                    {
                        pos.value.x - bar.rectangle.width / 2.f,
                        pos.value.y - bar.rectangle.height - renderable.texture.height,
                        bar.rectangle.width,
                        bar.rectangle.height
                    }, "", "", &health.value, 0,
                    health.max);
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
    world.import<gui::GUIModule>();
}
