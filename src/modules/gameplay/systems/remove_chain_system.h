//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_CHAIN_SYSTEM_H
#define REMOVE_CHAIN_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay {
    inline void remove_chain_system(const flecs::world& world, flecs::entity e) {
        e.remove<Chain>();
        ecs_delete_empty_tables_desc_t desc;
        desc.delete_generation = true;
        ecs_delete_empty_tables(world.c_ptr(), &desc);
    }
}
#endif //REMOVE_CHAIN_SYSTEM_H
