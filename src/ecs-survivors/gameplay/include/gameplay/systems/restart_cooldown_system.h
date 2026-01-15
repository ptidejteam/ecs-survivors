//
// Created by laurent on 29/05/25.
//

#ifndef RESTART_COOLDOWN_SYSTEM_H
#define RESTART_COOLDOWN_SYSTEM_H
#include <flecs.h>

#include "../components.h"

namespace gameplay::systems {
    inline void restart_cooldown_system(flecs::entity e) {
        if (e.has<Cooldown>())
            e.set<Cooldown>({e.get<Cooldown>().value, 0.0f});
    }
}
#endif //RESTART_COOLDOWN_SYSTEM_H
