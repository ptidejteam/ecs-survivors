//
// Created by laurent on 29/05/25.
//

#ifndef DECREMENT_MULTIPROJ_SYSTEM_H
#define DECREMENT_MULTIPROJ_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void decrement_multiproj_system(MultiProj& multi_proj) {
        multi_proj.projectile_count = std::max(multi_proj.projectile_count - 1, 1);
        multi_proj.spread_angle = std::max(multi_proj.spread_angle - 15.0f, multi_proj.min_spread);
    }
}
#endif //DECREMENT_MULTIPROJ_SYSTEM_H
