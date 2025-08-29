//
// Created by laurent on 29/05/25.
//

#ifndef ADD_CHAIN_SYSTEM_H
#define ADD_CHAIN_SYSTEM_H
#include <flecs.h>

#include "core/systems/remove_empty_tables_system.h"
#include "../components.h"

namespace gameplay::systems {
    inline void add_chain_system(const flecs::world& world, flecs::entity e) {
        e.remove<Pierce>();
        e.set<Chain>({1, std::unordered_set<int>()});
        core::systems::remove_empty_tables_system(world);
    }
}
#endif //ADD_CHAIN_SYSTEM_H
