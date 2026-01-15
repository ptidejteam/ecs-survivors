//
// Created by laurent on 30/05/25.
//

#ifndef CREATE_HEALTH_BAR_SYSTEM_H
#define CREATE_HEALTH_BAR_SYSTEM_H
#include <flecs.h>

#include "rendering/components.h"
#include "../components.h"

namespace gameplay::systems {
    inline void create_health_bar_system(flecs::entity e, const Health health) {
        e.add<HealthBar>();
        e.child()
                .set<Rectangle>({0, 0, 50, 10})
                .set<rendering::ProgressBar>({0, health.max, health.value});
    }
}
#endif //CREATE_HEALTH_BAR_SYSTEM_H
