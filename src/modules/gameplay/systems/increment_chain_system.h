//
// Created by laurent on 29/05/25.
//

#ifndef INCREMENT_CHAIN_SYSTEM_H
#define INCREMENT_CHAIN_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay {
    inline void increment_chain_system(Chain& c) {
        c.chain_count = c.chain_count + 1;
    }
}
#endif //INCREMENT_CHAIN_SYSTEM_H
