//
// Created by laure on 1/27/2025.
//

#include "rendering_module.h"

#include "components.h"
#include "pipeline_steps.h"
#include "modules/engine/core/components.h"
#include "gui/components.h"
#include <raymath.h>

#include "queries.h"
#include "gui/gui_module.h"
#include "modules/gameplay/components.h"
#include "systems/begin_drawing_system.h"
#include "systems/determine_visible_entities_system.h"
#include "systems/draw_entity_with_texture_system.h"
#include "systems/draw_health_bar_system.h"
#include "systems/end_drawing_system.h"


void rendering::RenderingModule::register_components(flecs::world world) {
    world.component<Priority>();
}

void rendering::RenderingModule::register_queries(flecs::world world) {
    auto base_query = world.query_builder<Renderable>();
    queries::entity_count_query = base_query.build();
    queries::entity_visible_count_query = base_query.with<Visible>().build();
}

void rendering::RenderingModule::register_systems(flecs::world world) {
    world.system("Before Draw")
            .kind<PreRender>()
            .run(systems::begin_drawing_system);

    world.system<TrackingCamera>("on start begin camera mode")
            .term_at(0).singleton()
            .kind(flecs::OnStart)
            .each([](TrackingCamera &camera) {
                camera.camera.target = camera.target.get<core::Position2D>()->value;
                camera.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
                camera.camera.zoom = 1.0f;
                camera.camera.rotation = 0;
            });

    world.system<TrackingCamera, core::GameSettings>("begin camera mode")
            .term_at(0).singleton()
            .term_at(1).singleton()
            .kind<PreRender>()
            .each([](TrackingCamera &camera, core::GameSettings &settings) {
                camera.camera.target = camera.target.get<core::Position2D>()->value;
                camera.camera.offset.x = settings.windowWidth / 2.0f;
                camera.camera.offset.y = settings.windowHeight / 2.0f;
                BeginMode2D(camera.camera);
            });

    world.system<const core::Position2D, const Renderable, const core::GameSettings, const TrackingCamera>("Determine Visible Entities")
            .term_at(2).singleton()
            .term_at(3).singleton()
            .write<Visible>()
            .kind<PreRender>()
            .multi_threaded()
            .each(systems::determine_visible_entities_system);

    world.system<const Renderable, const core::Position2D, const Rotation *>("Draw Entities with Textures")
            .kind<Render>()
            .with<Visible>()
            .with<Priority>()
            .order_by<Priority>([](flecs::entity_t e1, const Priority *p1, flecs::entity_t e2, const Priority *p2) {
                int order = p1->priority - p2->priority;
                if (order == 0)
                    return static_cast<int>(e1 - e2);
                return p1->priority - p2->priority;
            })
            .each(systems::draw_entity_with_texture_system);


    world.system<ProgressBar, const core::Position2D, const Renderable>("show healthbar")
            .kind<Render>()
            .each(systems::draw_health_bar_system);

    world.system("end camera mode")
            .kind<Render>().run([&](flecs::iter &it) {
                EndMode2D();
            });

    world.system("After Draw")
            .kind<PostRender>()
            .run(systems::end_drawing_system);
}

void rendering::RenderingModule::register_pipeline(flecs::world world) {
    world.component<PreRender>().add(flecs::Phase).depends_on(flecs::OnStore);
    world.component<RenderBackground>().add(flecs::Phase).depends_on<PreRender>();
    world.component<RenderGizmos>().add(flecs::Phase).depends_on<RenderBackground>();
    world.component<Render>().add(flecs::Phase).depends_on<RenderGizmos>();
    world.component<RenderGUI>().add(flecs::Phase).depends_on<Render>();
    world.component<PostRender>().add(flecs::Phase).depends_on<RenderGUI>();
}

void rendering::RenderingModule::register_submodules(flecs::world world) {
    world.import<gui::GUIModule>();
}
