//
// Created by laurent on 20/06/25.
//

#ifndef INCREMENT_BOUNCE_SYSTEM_H
#define INCREMENT_BOUNCE_SYSTEM_H
namespace gameplay::systems {
    inline void increment_bounce_system(Bounce &bounce) {
        bounce.bounce_count += 1;
    }
}
#endif //INCREMENT_BOUNCE_SYSTEM_H
