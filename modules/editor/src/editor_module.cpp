//
// Created by laurent on 8/30/25.
//

#include "editor/editor_module.h"
#include "editor/systems/init_imgui_system.h"

void editor::EditorModule::register_components(flecs::world &world) {

}

void editor::EditorModule::register_systems(flecs::world &world) {
    world.system("init imgui")
        .kind(flecs::OnStart)
        .run(systems::init_imgui_system);
}

void editor::EditorModule::register_entities(flecs::world &world) {

}
