//
// Created by laurent on 20/06/25.
//

#ifndef ADD_BOUNCE_SYSTEM_H
#define ADD_BOUNCE_SYSTEM_H
namespace gameplay::systems {
    inline void add_bounce_system(flecs::entity e) {
        e.set<Bounce>({1});
    }
}
#endif //ADD_BOUNCE_SYSTEM_H
