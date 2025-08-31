//
// Created by laurent on 30/05/25.
//

#ifndef UPDATE_HEALTH_BAR_SYSTEM_H
#define UPDATE_HEALTH_BAR_SYSTEM_H
#include "rendering/components.h"
#include "../components.h"

namespace gameplay::systems {
    inline void update_health_bar_system(const Health &health, rendering::ProgressBar &health_bar) {
        health_bar.current_val = health.value;
    }
}
#endif //UPDATE_HEALTH_BAR_SYSTEM_H
