//
// Created by laurent on 29/05/25.
//

#ifndef REMOVE_MULTPROJ_SYSTEM_H
#define REMOVE_MULTPROJ_SYSTEM_H
#include "../components.h"

namespace gameplay::systems {
    inline void remove_multiproj_system(flecs::entity e) {
        e.remove<MultiProj>();
    }
}
#endif //REMOVE_MULTPROJ_SYSTEM_H
