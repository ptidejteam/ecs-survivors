//
// Created by laurent on 29/05/25.
//

#ifndef PROJECTILE_NO_EFFECT_COLLIDED_SYSTEM_H
#define PROJECTILE_NO_EFFECT_COLLIDED_SYSTEM_H

#include <flecs.h>

#include "core/components.h"

namespace gameplay::systems {
    inline void project_no_effect_collided_system(flecs::iter &it, size_t i) {
        it.entity(i).add<core::DestroyAfterFrame>();
    }
}
#endif //PROJECTILE_NO_EFFECT_COLLIDED_SYSTEM_H
