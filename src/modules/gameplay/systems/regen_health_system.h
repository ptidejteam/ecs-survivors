//
// Created by laurent on 29/05/25.
//

#ifndef REGEN_HEALTH_SYSTEM_H
#define REGEN_HEALTH_SYSTEM_H

#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay {
    inline void regen_health_system(flecs::iter &it, size_t i, Health &health, RegenHealth &regen) {
        health.value = std::min(health.value + regen.rate * it.delta_time(), health.max);
    }
}
#endif //REGEN_HEALTH_SYSTEM_H
