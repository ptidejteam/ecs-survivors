//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_EMPTY_TABLES_SYSTEM_H
#define REMOVE_EMPTY_TABLES_SYSTEM_H
#include <flecs.h>

namespace core::systems {
    inline void remove_empty_tables_system(const flecs::world& world) {
        ecs_delete_empty_tables_desc_t desc;
        desc.delete_generation = true;
        ecs_delete_empty_tables(world.c_ptr(), &desc);}
    //std::printf("Removing empty tables to avoid fragmentation in collision\n");
}
#endif //REMOVE_EMPTY_TABLES_SYSTEM_H
