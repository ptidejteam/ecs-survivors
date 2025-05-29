//
// Created by laurent on 29/05/25.
//

#ifndef DESTROY_ENTITY_AFTER_TIME_SYSTEM_H
#define DESTROY_ENTITY_AFTER_TIME_SYSTEM_H

#include <flecs.h>
#include "modules/engine/core/components.h"
namespace core {
    void destroy_entity_after_time_system(flecs::iter& it, size_t i, DestroyAfterTime& time) {
        time.time -= it.delta_time();
        if(time.time <= 0.0f) it.entity(i).add<DestroyAfterFrame>();
    }
}

#endif //DESTROY_ENTITY_AFTER_TIME_SYSTEM_H
