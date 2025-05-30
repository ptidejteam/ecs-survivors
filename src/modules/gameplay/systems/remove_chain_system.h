//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_CHAIN_SYSTEM_H
#define REMOVE_CHAIN_SYSTEM_H
#include <flecs.h>

#include "modules/engine/core/systems/remove_empty_tables_system.h"
#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void remove_chain_system(const flecs::world& world, flecs::entity e) {
        e.remove<Chain>();
        core::systems::remove_empty_tables_system(world);
    }
}
#endif //REMOVE_CHAIN_SYSTEM_H
