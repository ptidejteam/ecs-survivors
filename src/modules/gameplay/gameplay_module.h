//
// Created by laure on 3/12/2025.
//

#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "modules/base_module.h"
#include <flecs.h>

namespace gameplay {
    constexpr float SPAWNER_INTERVAL = 0.1f;
    class GameplayModule : public BaseModule<GameplayModule> {
    public:
        GameplayModule(flecs::world &world): BaseModule(world) {}

    private:

        flecs::entity m_spawner_tick;

        void register_components(flecs::world);

        void register_systems(flecs::world);

        friend class BaseModule<GameplayModule>;
    };
}

#endif //GAMEPLAY_MODULE_H
