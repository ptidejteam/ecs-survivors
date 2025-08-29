//
// Created by Laurent on 7/21/2025.
//

#ifndef DISABLE_CHILDREN_ON_DISABLE_SYSTEM_H
#define DISABLE_CHILDREN_ON_DISABLE_SYSTEM_H
#include <flecs.h>
#include <raylib.h>

namespace rendering::gui::systems {
    inline void disable_children_on_disable_system(flecs::entity e) {
        e.children([](flecs::entity child) {
            if (child.has<Rectangle>())
                child.disable();
        });
    }

}
#endif //DISABLE_CHILDREN_ON_DISABLE_SYSTEM_H
