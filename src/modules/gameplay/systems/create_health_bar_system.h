//
// Created by laurent on 30/05/25.
//

#ifndef CREATE_HEALTH_BAR_SYSTEM_H
#define CREATE_HEALTH_BAR_SYSTEM_H
#include <flecs.h>

#include "modules/engine/rendering/components.h"
#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void create_health_bar_system(flecs::entity e, const gameplay::Health health) {
        e.set<rendering::ProgressBar>({0, health.max, health.value, 0, 0, 50, 10});
    }
}
#endif //CREATE_HEALTH_BAR_SYSTEM_H
