//
// Created by Laurent Voisard on 1/10/2025.
//

#ifndef PLAYER_MODULE_H
#define PLAYER_MODULE_H

#include "modules/base_module.h"

namespace player {
    class PlayerModule : public BaseModule<PlayerModule> {
    public:
        PlayerModule(flecs::world world) : BaseModule(world) {};

    private:
        void register_components(flecs::world world);

        void register_systems(flecs::world world);

        friend class BaseModule<PlayerModule>;
    };
}


#endif //PLAYER_MODULE_H
