//
// Created by Laurent on 7/21/2025.
//

#ifndef SET_PAUSED_ON_ENTITY_DISABLE_SYSTEM_H
#define SET_PAUSED_ON_ENTITY_DISABLE_SYSTEM_H

#include <flecs.h>
#include "core/components.h"


namespace core::systems {
    inline void set_paused_on_entity_disable_system(flecs::iter &it, size_t i, EnabledMenus &enabled) {
        enabled.count --;
        if (enabled.count == 0) {
            it.world().set<Paused>({false});
        }
    }
}
#endif //SET_PAUSED_ON_ENTITY_DISABLE_SYSTEM_H
