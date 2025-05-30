//
// Created by laurent on 30/05/25.
//

#ifndef ADD_COLLIDED_WITH_SYSTEM_H
#define ADD_COLLIDED_WITH_SYSTEM_H
#include <flecs.h>
#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void add_collided_with_system(const CollisionRecordList& list) {
        for (CollisionRecord rec: list.significant_collisions) {
            rec.a.add<CollidedWith>(rec.b);
            rec.b.add<CollidedWith>(rec.a);
        }
    }
}
#endif //ADD_COLLIDED_WITH_SYSTEM_H
