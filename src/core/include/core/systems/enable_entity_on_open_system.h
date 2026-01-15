//
// Created by Laurent on 7/21/2025.
//

#ifndef DISABLE_ENTITY_ON_ENABLE_SYSTEM_H
#define DISABLE_ENTITY_ON_ENABLE_SYSTEM_H

#include <flecs.h>
#include "core/components.h"

namespace core::systems {
    inline void enable_entity_on_open_system(flecs::entity e) {
        e.remove<Open>();
        e.enable();
    }
}
#endif //DISABLE_ENTITY_ON_ENABLE_SYSTEM_H
