//
// Created by Laurent on 7/21/2025.
//

#ifndef CHECK_IF_DEAD_SYSTEM_H
#define CHECK_IF_DEAD_SYSTEM_H

#include <flecs.h>
#include "../components.h"
#include "core/components.h"

namespace gameplay::systems {
    inline void check_if_dead_system(flecs::entity e, const Health &h) {
        if (h.value <= 0.0f) {
            e.add<core::DestroyAfterFrame>();
        }
    }
}

#endif //CHECK_IF_DEAD_SYSTEM_H
