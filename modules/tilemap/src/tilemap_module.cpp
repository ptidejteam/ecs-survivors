//
// Created by laurent on 03/06/25.
//

#include "tilemap/tilemap_module.h"

#include "tilemap/components.h"
#include "rendering/pipeline_steps.h"
#include "rendering/rendering_module.h"
#include "tilemap/systems/create_tilemap_system.h"


namespace tilemap {
    void TilemapModule::register_components(flecs::world world) {
    }

    void TilemapModule::register_systems(flecs::world world) {
        world.system<const Tilemap>("create tilemaps")
                .kind(flecs::OnStart)
                .each(systems::create_tilemap_system);

    }
}
