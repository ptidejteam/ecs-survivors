//
// Created by laurent on 29/05/25.
//

#ifndef COLLISION_CLEANUP_SYSTEM_H
#define COLLISION_CLEANUP_SYSTEM_H

#include <flecs.h>

#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void collision_cleanup_system(flecs::iter &it, size_t i) {
        it.world().remove_all<CollidedWith>(it.entity(i));
    }

    inline void collision_cleanup_list_system(CollisionRecordList& list) {
        list.records.clear();
        list.significant_collisions.clear();
        list.collisions_info.clear();
    }
}

#endif //COLLISION_CLEANUP_SYSTEM_H
