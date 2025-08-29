//
// Created by laurent on 03/06/25.
//

#include "tilemap_module.h"

#include "components.h"
#include "modules/engine/rendering/pipeline_steps.h"
#include "modules/engine/rendering/rendering_module.h"
#include "systems/create_tilemap_system.h"


namespace tilemap {
    void TilemapModule::register_components(flecs::world world) {
    }

    void TilemapModule::register_systems(flecs::world world) {
        world.system<const Tilemap>("create tilemaps")
                .kind(flecs::OnStart)
                .each(systems::create_tilemap_system);

    }
}
