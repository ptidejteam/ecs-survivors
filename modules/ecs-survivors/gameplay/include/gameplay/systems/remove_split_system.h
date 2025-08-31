//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_SPLIT_SYSTEM_H
#define REMOVE_SPLIT_SYSTEM_H
#include "../components.h"

namespace gameplay::systems {
    inline void remove_split_system(const flecs::world& world, flecs::entity e) {
        e.remove<Split>();
        core::systems::remove_empty_tables_system(world);
    }
}
#endif //REMOVE_SPLIT_SYSTEM_H
