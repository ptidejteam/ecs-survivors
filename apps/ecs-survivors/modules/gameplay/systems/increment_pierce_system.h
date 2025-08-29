//
// Created by laurent on 29/05/25.
//

#ifndef INCREMENT_PIERCE_SYSTEM_H
#define INCREMENT_PIERCE_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void increment_pierce_system(Pierce& p) {
        p.pierce_count = p.pierce_count + 1;
    }
}
#endif //INCREMENT_PIERCE_SYSTEM_H
