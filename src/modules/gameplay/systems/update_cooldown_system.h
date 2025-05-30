//
// Created by laurent on 29/05/25.
//

#ifndef UPDATE_COOLDOWN_SYSTEM_H
#define UPDATE_COOLDOWN_SYSTEM_H
#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void update_cooldown_system(flecs::iter &it, size_t i, Cooldown &cooldown) {
        cooldown.elapsed_time += it.delta_time();
        if (cooldown.elapsed_time > cooldown.value)
            it.entity(i).add<CooldownCompleted>();
    }
}

#endif //UPDATE_COOLDOWN_SYSTEM_H
