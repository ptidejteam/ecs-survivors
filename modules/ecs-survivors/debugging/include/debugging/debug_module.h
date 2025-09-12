//
// Created by Laurent on 4/18/2025.
//

#ifndef DEBUG_MODULE_H
#define DEBUG_MODULE_H
#include "base/base_module.h"


namespace debugging {
    class DebugModule : public base::BaseModule<DebugModule> {
    public:
        DebugModule(flecs::world world): BaseModule(world) {
        };
    private:
        flecs::system debug_entity_ids;
        flecs::system debug_collider_bounds;
        flecs::system debug_circle_colliders;
        flecs::system debug_square_colliders;
        flecs::system debug_FPS;
        flecs::system debug_entity_count;
        flecs::system debug_mouse_pos;
        flecs::system debug_grid;
        flecs::system debug_closest_enemy;

        flecs::system grid_cell_grow;
        flecs::system grid_cell_shrink;

        void register_components(flecs::world& world);

        void register_systems(flecs::world& world);

        void register_entities(flecs::world& world);

        friend class BaseModule<DebugModule>;
    };
}


#endif //DEBUG_MODULE_H
