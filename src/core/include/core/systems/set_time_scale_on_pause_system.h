//
// Created by Laurent on 7/21/2025.
//

#ifndef SET_TIME_SCALE_ON_PAUSE_SYSTEM_H
#define SET_TIME_SCALE_ON_PAUSE_SYSTEM_H

#include <flecs.h>
#include "core/components.h"

namespace core::systems {
    inline void set_time_scale_on_pause_system(flecs::iter &it, size_t i, const Paused &paused) {
        it.world().set_time_scale(!paused.paused);
    }
}

#endif //SET_TIME_SCALE_ON_PAUSE_SYSTEM_H
