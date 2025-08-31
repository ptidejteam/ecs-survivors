//
// Created by laure on 1/27/2025.
//

#include <raylib.h>
#include <raymath.h>

#include "rendering/rendering_module.h"

#include "core/components.h"
#include "rendering/components.h"
#include "rendering/pipeline_steps.h"

#include "rendering/queries.h"
#include "rendering/systems/begin_drawing_system.h"
#include "rendering/systems/create_camera_system.h"
#include "rendering/systems/determine_visible_entities_system.h"
#include "rendering/systems/draw_entity_with_texture_system.h"
#include "rendering/systems/draw_health_bar_system.h"
#include "rendering/systems/end_drawing_system.h"
#include "rendering/systems/update_and_begin_camera_mode_system.h"


void rendering::RenderingModule::register_components(flecs::world world) { world.component<Priority>(); }

void rendering::RenderingModule::register_queries(flecs::world world) {
    auto base_query = world.query_builder<Renderable>();
    queries::entity_count_query = base_query.build();
    queries::entity_visible_count_query = base_query.with<Visible>().build();
}

void rendering::RenderingModule::register_systems(flecs::world world) {

    world.system<TrackingCamera>("on start init camera")
            .term_at(0)
            .singleton()
            .kind(flecs::OnStart)
            .each(systems::create_camera_system);

    world.system("Before Draw").kind<PreRender>().run(systems::begin_drawing_system);


    world.system<const core::Position2D, const Renderable, const Settings, const TrackingCamera>(
                 "Determine Visible Entities")
            .term_at(2)
            .singleton()
            .term_at(3)
            .singleton()
            .write<Visible>()
            .kind<PreRender>()
            .multi_threaded()
            .each(systems::determine_visible_entities_system);

    world.system<const Viewport>("draw on viewport")
        .kind<RenderStart>().each([] (const Viewport& vp) {
            BeginTextureMode(vp.render_target);
        });

    world.system<TrackingCamera, Settings>("begin camera mode")
                .term_at(0)
                .singleton()
                .term_at(1)
                .singleton()
                .kind<RenderStart>()
                .each(systems::update_and_begin_camera_mode_system);
    world.system<const Renderable>("Draw Background Textures")
            .kind<RenderStart>()
            .without<core::Position2D>()
            .with<Priority>()
            .order_by<Priority>([](flecs::entity_t e1, const Priority *p1, flecs::entity_t e2, const Priority *p2) {
                int order = p1->priority - p2->priority;
                if (order == 0)
                    return static_cast<int>(e1 - e2);
                return p1->priority - p2->priority;
            })
            .each(systems::draw_background_textures_system);

    world.system<const Renderable, const core::Position2D, const Rotation *>("Draw Entities with Textures")
            .kind<RenderObjects>()
            .with<Visible>()
            .with<Priority>()
            .order_by<Priority>([](flecs::entity_t e1, const Priority *p1, flecs::entity_t e2, const Priority *p2) {
                int order = p1->priority - p2->priority;
                if (order == 0)
                    return static_cast<int>(e1 - e2);
                return p1->priority - p2->priority;
            })
            .each(systems::draw_entity_with_texture_system);


    world.system<ProgressBar, Rectangle, const core::Position2D, const Renderable>("show healthbar")
            .term_at(2)
            .parent()
            .term_at(3)
            .parent()
            .kind<RenderObjects>()
            .each(systems::draw_health_bar_system);


    world.system("end camera mode").kind<RenderGizmos>().run(systems::end_camera_mode_system);
    world.system<const Viewport>().kind<RenderEnd>().each([] (const Viewport&) {
       EndTextureMode();
    });

    world.system("After Draw").kind<PostRender>().run(systems::end_drawing_system);
}

void rendering::RenderingModule::register_pipeline(flecs::world world) {
    world.component<PreRender>().add(flecs::Phase).depends_on(flecs::OnStore);
    world.component<RenderStart>().add(flecs::Phase).depends_on<PreRender>();
    world.component<RenderObjects>().add(flecs::Phase).depends_on<RenderStart>();
    world.component<RenderGizmos>().add(flecs::Phase).depends_on<RenderObjects>();
    world.component<RenderGUI>().add(flecs::Phase).depends_on<RenderGizmos>();
    world.component<RenderEnd>().add(flecs::Phase).depends_on<RenderGUI>();
    world.component<PostRender>().add(flecs::Phase).depends_on<RenderEnd>();
}

void rendering::RenderingModule::register_submodules(flecs::world world) {}
