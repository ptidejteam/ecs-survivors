//
// Created by laure on 3/12/2025.
//

#ifndef GAMEPLAY_COMPONENTS_H
#define GAMEPLAY_COMPONENTS_H

#include <flecs.h>

namespace gameplay {

    struct Spawner {
        std::string enemy_prefab_name;
    };

    struct TakeDamage {
        float damage;
    };

    struct RegenHealth {
        float rate; // per second
    };

}
#endif //GAMEPLAY_COMPONENTS_H
