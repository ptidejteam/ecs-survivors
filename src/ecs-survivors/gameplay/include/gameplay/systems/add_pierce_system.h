//
// Created by laurent on 29/05/25.
//

#ifndef ADD_PIERCE_SYSTEM_H
#define ADD_PIERCE_SYSTEM_H
#include <flecs.h>

#include "core/systems/remove_empty_tables_system.h"
#include "../components.h"

namespace gameplay::systems {
    inline void add_pierce_system(const flecs::world& world, flecs::entity e) {
        e.remove<Chain>();
        e.set<Pierce>({1, std::unordered_set<int>()});
        core::systems::remove_empty_tables_system(world);
    }
}
#endif //ADD_PIERCE_SYSTEM_H
