//
// Created by laurent on 30/05/25.
//

#ifndef ADD_COLLIDED_WITH_SYSTEM_H
#define ADD_COLLIDED_WITH_SYSTEM_H
#include <flecs.h>
#include "modules/engine/physics/components.h"

namespace physics::systems {
    inline void add_collided_with_system(CollisionRecordList& list) {

        for (SignificantCollisionRecord rec: list.significant_collisions) {
            rec.a.add<CollidedWith>(rec.b);
            rec.b.add<CollidedWith>(rec.a);
            list.collisions_info[{rec.a.id(),rec.b.id()}] = rec.a_info;
            list.collisions_info[{rec.b.id(), rec.a.id()}] = rec.b_info;
        }
    }
}
#endif //ADD_COLLIDED_WITH_SYSTEM_H
