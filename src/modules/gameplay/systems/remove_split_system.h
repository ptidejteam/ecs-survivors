//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_SPLIT_SYSTEM_H
#define REMOVE_SPLIT_SYSTEM_H
#include "modules/gameplay/components.h"

namespace gameplay {
    inline void remove_split_system(const flecs::world& world, flecs::entity e) {
        e.remove<Split>();
        ecs_delete_empty_tables_desc_t desc;
        desc.delete_generation = true;
        ecs_delete_empty_tables(world.c_ptr(), &desc);
    }
}
#endif //REMOVE_SPLIT_SYSTEM_H
