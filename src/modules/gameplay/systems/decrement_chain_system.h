//
// Created by laurent on 29/05/25.
//

#ifndef DECREMENT_CHAIN_SYSTEM_H
#define DECREMENT_CHAIN_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay {
    inline void decrement_chain_system(Chain& chain) {
        chain.chain_count = std::max(chain.chain_count - 1, 1);
    }
}
#endif //DECREMENT_CHAIN_SYSTEM_H
