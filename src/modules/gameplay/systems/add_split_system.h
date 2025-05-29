//
// Created by laurent on 29/05/25.
//

#ifndef ADD_SPLIT_SYSTEM_H
#define ADD_SPLIT_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay {
    inline void add_split_system(const flecs::world& world, flecs::entity e) {
        e.set<Split>({std::unordered_set<int>()});
        ecs_delete_empty_tables_desc_t desc;
        desc.delete_generation = true;
        ecs_delete_empty_tables(world.c_ptr(), &desc);
    }
}
#endif //ADD_SPLIT_SYSTEM_H
