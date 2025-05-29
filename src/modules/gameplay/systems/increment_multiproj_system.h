//
// Created by laurent on 29/05/25.
//

#ifndef INCREMENT_MULTIPROJ_SYSTEM_H
#define INCREMENT_MULTIPROJ_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay {
    inline void increment_multiproj_system(MultiProj& multi_proj) {
        multi_proj.projectile_count += 1;
        multi_proj.spread_angle = std::min(multi_proj.spread_angle + 15.0f, multi_proj.max_spread);
    }
}
#endif //INCREMENT_MULTIPROJ_SYSTEM_H
