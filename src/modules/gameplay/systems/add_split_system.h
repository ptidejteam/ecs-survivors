//
// Created by laurent on 29/05/25.
//

#ifndef ADD_SPLIT_SYSTEM_H
#define ADD_SPLIT_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void add_split_system(const flecs::world& world, flecs::entity e) {
        e.set<Split>({std::unordered_set<int>()});
        core::systems::remove_empty_tables_system(world);
    }
}
#endif //ADD_SPLIT_SYSTEM_H
