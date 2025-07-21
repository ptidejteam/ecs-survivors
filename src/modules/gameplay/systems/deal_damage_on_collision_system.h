//
// Created by laurent on 29/05/25.
//

#ifndef DEAL_DAMAGE_ON_COLLISION_SYSTEM_H
#define DEAL_DAMAGE_ON_COLLISION_SYSTEM_H
#include <flecs.h>

#include "modules/gameplay/components.h"

namespace gameplay::systems {
    inline void deal_damage_on_collision_system(flecs::iter &it, size_t i, Damage &dmg) {
        flecs::entity self = it.entity(i);
        flecs::entity other = it.pair(1).second();

        if (!other.has<Health>()) return;
        float value = dmg.value;
        if (other.has<TakeDamage>()) {
            value += other.get<TakeDamage>().damage;
        }
        other.set<TakeDamage>({value});
    }
}
#endif //DEAL_DAMAGE_ON_COLLISION_SYSTEM_H
