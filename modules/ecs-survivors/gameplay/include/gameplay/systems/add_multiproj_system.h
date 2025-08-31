//
// Created by laurent on 29/05/25.
//

#ifndef ADD_MULTIPROJ_SYSTEM_H
#define ADD_MULTIPROJ_SYSTEM_H
#include <flecs.h>

#include "../components.h"

namespace gameplay::systems {
    inline void add_multiproj_system(flecs::entity e) {
        e.set<MultiProj>({2, 30,150, 30});
    }
}
#endif //ADD_MULTIPROJ_SYSTEM_H
