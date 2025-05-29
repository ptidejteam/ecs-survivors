//
// Created by laure on 1/27/2025.
//

#include "rendering_module.h"

#include "components.h"
#include "pipeline_steps.h"
#include "modules/engine/core/components.h"
#include "gui/components.h"
#include <raymath.h>

#include "gui/gui_module.h"
#include "modules/gameplay/components.h"
#include "systems/begin_drawing_system.h"
#include "systems/determine_visible_entities_system.h"
#include "systems/draw_entity_with_texture_system.h"
#include "systems/draw_health_bar_system.h"
#include "systems/end_drawing_system.h"


void rendering::RenderingModule::register_components(flecs::world world) {
    world.component<Circle>();
    world.component<Priority>();
}

void rendering::RenderingModule::register_systems(flecs::world world) {

    world.system("Before Draw")
            .kind<PreRender>()
            .run(begin_drawing_system);

    world.system<const core::Position2D, const Renderable, const core::GameSettings>("Determine Visible Entities")
            .term_at(2).singleton()
            .write<Visible>()
            .kind<PreRender>()
            .each(determine_visible_entities_system);

    world.system<const Renderable, const core::Position2D, const Rotation *>("Draw Entities with Textures")
            .kind<Render>()
            .with<Visible>()
            .group_by<Priority>()
            .each(draw_entity_with_texture_system);

    world.system<const HealthBar, gameplay::Health, const core::Position2D, const Renderable>("show healthbar")
            .kind<Render>()
            .each(draw_health_bar_system);

    world.system("After Draw")
            .kind<PostRender>()
            .run(end_drawing_system);
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
