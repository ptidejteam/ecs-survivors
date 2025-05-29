//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_PIERCE_SYSTEM_H
#define REMOVE_PIERCE_SYSTEM_H
#include "modules/gameplay/components.h"

namespace gameplay {
    inline void remove_pierce_system(const flecs::world& world, flecs::entity e) {
        e.remove<Pierce>();
        ecs_delete_empty_tables_desc_t desc;
        desc.delete_generation = true;
        ecs_delete_empty_tables(world.c_ptr(), &desc);
    }
}
#endif //REMOVE_PIERCE_SYSTEM_H
