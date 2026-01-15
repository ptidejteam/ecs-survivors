//
// Created by laurent on 29/05/25.
//

#ifndef DECREMENT_PIERCE_SYSTEM_H
#define DECREMENT_PIERCE_SYSTEM_H
#include <flecs.h>

#include "../components.h"

namespace gameplay::systems {
    inline void decrement_pierce_system(Pierce& p) {
        p.pierce_count = std::max(1, p.pierce_count - 1);
    }
}
#endif //DECREMENT_PIERCE_SYSTEM_H
