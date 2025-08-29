//
// Created by Laurent on 7/21/2025.
//

#ifndef SET_PAUSED_ON_ENTITY_ENABLED_SYSTEM_H
#define SET_PAUSED_ON_ENTITY_ENABLED_SYSTEM_H

#include <flecs.h>
#include "modules/engine/core/components.h"
namespace core::systems {
    inline void set_paused_on_entity_enabled_system(flecs::iter &it, size_t i, EnabledMenus &enabled) {
        enabled.count ++;
        it.world().set<Paused>({true});
    }
}
#endif //SET_PAUSED_ON_ENTITY_ENABLED_SYSTEM_H
