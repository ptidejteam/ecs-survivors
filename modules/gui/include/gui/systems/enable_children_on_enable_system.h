//
// Created by Laurent on 7/21/2025.
//

#ifndef ENABLE_CHILDREN_ON_ENABLE_SYSTEMS_H
#define ENABLE_CHILDREN_ON_ENABLE_SYSTEMS_H
#include <flecs.h>
#include <raylib.h>

namespace rendering::gui::systems {
    inline void enable_children_on_enable_system(flecs::entity e) {
        e.children([](flecs::entity child) {
            if (child.has<Rectangle>())
                child.enable();
        });
    }
}
#endif //ENABLE_CHILDREN_ON_ENABLE_SYSTEMS_H
