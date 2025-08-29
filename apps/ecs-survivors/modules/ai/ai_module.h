//
// Created by Laurent Voisard on 1/10/2025.
//

#ifndef AI_MODULE_H
#define AI_MODULE_H

#include "modules/base_module.h"

namespace ai {
    class AIModule : public BaseModule<AIModule> {
    public:
        AIModule(flecs::world world): BaseModule(world) {
        };

    private:
        void register_components(flecs::world);

        void register_systems(flecs::world);

        friend class BaseModule<AIModule>;
    };
}


#endif //AI_MODULE_H
