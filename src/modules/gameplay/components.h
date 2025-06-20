//
// Created by laure on 3/12/2025.
//

#include <unordered_set>
#ifndef GAMEPLAY_COMPONENTS_H
#define GAMEPLAY_COMPONENTS_H

#include <flecs.h>

namespace gameplay {

    struct Spawner {
        flecs::entity enemy_prefab;
    };

    struct TakeDamage {
        float damage;
    };

    struct Damage {
        float value;
    };

    struct Health {
        float max;
        float value;
    };

    struct Attack {
        std::string attack_prefab_name;
        std::string target_tag;
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

    struct Bounce {
        int bounce_count;
    };

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

    struct MultiProj {
        int projectile_count;
        float spread_angle;
        float max_spread;
        float min_spread;
    };

}
#endif //GAMEPLAY_COMPONENTS_H
