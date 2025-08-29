//
// Created by laure on 3/12/2025.
//

#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "modules/base_module.h"
#include <flecs.h>

namespace gameplay {
    const float BASE_SPAWNER_INTERVAL = 0.33f;
    inline float spawner_interval = BASE_SPAWNER_INTERVAL;
    inline flecs::system add_multiproj;
    inline flecs::system remove_multiproj;
    inline flecs::system add_pierce;
    inline flecs::system remove_pierce;
    inline flecs::system add_chain;
    inline flecs::system remove_chain;
    inline flecs::system add_split;
    inline flecs::system remove_split;
    inline flecs::system add_proj;
    inline flecs::system remove_proj;
    inline flecs::system add_pierce_amt;
    inline flecs::system remove_pierce_amt;
    inline flecs::system add_chain_amt;
    inline flecs::system remove_chain_amt;

    inline flecs::system add_bounce;
    inline flecs::system remove_bounce;
    inline flecs::system add_bounce_amt;
    inline flecs::system remove_bounce_amt;
    inline flecs::system spawn_system;

    class GameplayModule : public BaseModule<GameplayModule> {
    public:
        GameplayModule(flecs::world &world): BaseModule(world) {
        }
        static inline flecs::entity m_spawner_tick;

    private:

        void register_components(flecs::world);

        void register_systems(flecs::world);

        void register_entities(flecs::world);

        void register_pipeline(flecs::world);

        friend class BaseModule<GameplayModule>;
    };
}

#endif //GAMEPLAY_MODULE_H
