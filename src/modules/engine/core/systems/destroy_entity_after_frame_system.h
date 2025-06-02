//
// Created by laurent on 29/05/25.
//

#ifndef DESTROY_ENTITY_AFTER_FRAME_SYSTEM_H
#define DESTROY_ENTITY_AFTER_FRAME_SYSTEM_H

#include <flecs.h>
#include "modules/engine/core/components.h"
namespace core::systems {
    inline void destroy_entity_after_frame_system(flecs::iter& it, size_t i) {
        it.entity(i).destruct();
    }
}

#endif //DESTROY_ENTITY_AFTER_FRAME_SYSTEM_H
