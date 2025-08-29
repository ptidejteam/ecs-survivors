//
// Created by laurent on 20/06/25.
//

#ifndef DECREMENT_BOUNCE_SYSTEM_H
#define DECREMENT_BOUNCE_SYSTEM_H
namespace gameplay::systems {
    inline void decrement_bounce_system(Bounce &bounce) {
        bounce.bounce_count = std::max(1, bounce.bounce_count - 1);
    }
}
#endif //DECREMENT_BOUNCE_SYSTEM_H
