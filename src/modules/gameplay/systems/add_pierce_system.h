//
// Created by laurent on 29/05/25.
//

#ifndef ADD_PIERCE_SYSTEM_H
#define ADD_PIERCE_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay {
    inline void add_pierce_system(const flecs::world& world, flecs::entity e) {
        e.remove<Chain>();
        e.set<Pierce>({1, std::unordered_set<int>()});
        ecs_delete_empty_tables_desc_t desc;
        desc.delete_generation = true;
        ecs_delete_empty_tables(world.c_ptr(), &desc);
    }
}
#endif //ADD_PIERCE_SYSTEM_H
