//
// Created by Laurent on 7/21/2025.
//

#ifndef DISABLE_ENTITY_ON_CLOSE_SYSTEM_H
#define DISABLE_ENTITY_ON_CLOSE_SYSTEM_H
#include <flecs.h>
#include "modules/engine/core/components.h"

namespace core::systems {
    inline void disable_entity_on_close_system(flecs::entity e) {
        e.remove<Close>();
        e.disable();
    }
}
#endif //DISABLE_ENTITY_ON_CLOSE_SYSTEM_H
