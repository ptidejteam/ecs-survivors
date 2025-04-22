//
// Created by laure on 3/12/2025.
//

#include <unordered_set>
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

    struct Cooldown {
        float value;
        float elapsed_time;
    };

    struct CooldownCompleted {};

    struct Projectile {};


    struct Pierce {
        int pierce_count;
        std::unordered_set<int> hits;
    };

    struct Chain {
        int chain_count;
        std::unordered_set<int> hits;
    };

    struct Split {
        std::unordered_set<int> hits;
    };

}
#endif //GAMEPLAY_COMPONENTS_H
