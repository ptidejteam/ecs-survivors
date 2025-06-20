//
// Created by laurent on 20/06/25.
//

#ifndef REMOVE_BOUNCE_SYSTEM_H
#define REMOVE_BOUNCE_SYSTEM_H
namespace gameplay::systems {
    inline void remove_bounce_system(flecs::entity e) {
        e.remove<Bounce>();
    }
}
#endif //REMOVE_BOUNCE_SYSTEM_H
