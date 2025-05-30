//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_PIERCE_SYSTEM_H
#define REMOVE_PIERCE_SYSTEM_H
#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void remove_pierce_system(const flecs::world& world, flecs::entity e) {
        e.remove<Pierce>();
        core::systems::remove_empty_tables_system(world);
    }
}
#endif //REMOVE_PIERCE_SYSTEM_H
