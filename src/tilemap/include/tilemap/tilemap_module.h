//
// Created by laurent on 03/06/25.
//

#ifndef TILEMAPMODULE_H
#define TILEMAPMODULE_H

#include <flecs.h>
#include "base/base_module.h"

namespace tilemap {
    class TilemapModule : public base::BaseModule<TilemapModule> {
    public:
        TilemapModule(flecs::world &world): BaseModule(world) {
        }
    private:
        void register_components(flecs::world world);

        void register_systems(flecs::world world);

        friend class BaseModule<TilemapModule>;
    };
}


#endif //TILEMAPMODULE_H
