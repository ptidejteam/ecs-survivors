//
// Created by laurent on 03/06/25.
//

#include "tilemap_module.h"

#include "components.h"
#include "modules/engine/rendering/pipeline_steps.h"
#include "modules/engine/rendering/rendering_module.h"
#include "systems/create_tilemap_system.h"
#include "systems/draw_tilemap_layer_tiles_system.h"


namespace tilemap {
    void TilemapModule::register_components(flecs::world world) {
    }

    void TilemapModule::register_systems(flecs::world world) {
        world.system<const Tilemap>("create tilemaps")
                .kind(flecs::OnStart)
                .each(systems::create_tilemap_system);

        world.system<TilemapLayer>("Draw Tilemap layer")
                .kind<rendering::RenderBackground>()
                .each(systems::draw_tilemap_layers_system);

        world.system<TilemapLayerTile>("Draw Tilemap layer tiles")
                .with<TilemapLayer>().parent()
                .kind<rendering::RenderBackground>()
                .each(systems::draw_tilemap_layer_tiles_system)
                .disable();
    }
}
