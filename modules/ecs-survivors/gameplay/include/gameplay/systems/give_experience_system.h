//
// Created by Laurent on 7/21/2025.
//

#ifndef GIVE_EXPERIENCE_SYSTEM_H
#define GIVE_EXPERIENCE_SYSTEM_H

#include "../components.h"

namespace gameplay::systems {
    inline void give_experience_system(GiveExperience &exp) {
        if (Experience *player_exp = exp.other.try_get_mut<Experience>(); player_exp) {
            player_exp->value += exp.value;
            if (player_exp->value >= player_exp->threshold) {
                //level up
                player_exp->level++;
                player_exp->value -= player_exp->threshold;
                player_exp->threshold = player_exp->threshold * 1.2f;
                exp.other.get_mut<Health>().value = exp.other.get<Health>().max;
                exp.other.emit<LevelUpEvent>({player_exp->level, player_exp->threshold});
            }
            exp.other.emit<ExpGainedEvent>({player_exp->value});
        }
    }
}
#endif //GIVE_EXPERIENCE_SYSTEM_H
