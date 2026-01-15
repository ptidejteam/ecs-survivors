//
// Created by laurent on 29/05/25.
//

#ifndef TAKE_DAMAGE_SYSTEM_H
#define TAKE_DAMAGE_SYSTEM_H
#include <flecs.h>

#include "core/components.h"
#include "../components.h"

namespace gameplay::systems {
    inline void take_damage_system(flecs::entity e, Health &health, TakeDamage &dmg) {
        health.value -= dmg.damage;
        e.remove<TakeDamage>();
    }
}
#endif //TAKE_DAMAGE_SYSTEM_H
