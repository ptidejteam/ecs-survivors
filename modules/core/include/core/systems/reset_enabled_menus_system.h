//
// Created by Laurent on 7/21/2025.
//

#ifndef RESET_ENABLED_MENUS_SYSTEM_H
#define RESET_ENABLED_MENUS_SYSTEM_H

#include "core/components.h"

namespace core::systems {
    inline void reset_enabled_menus_system(EnabledMenus& menus) {
        menus.count = 0;

    }
}

#endif //RESET_ENABLED_MENUS_SYSTEM_H
