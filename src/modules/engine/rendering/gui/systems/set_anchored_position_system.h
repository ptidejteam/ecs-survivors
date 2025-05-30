//
// Created by laurent on 29/05/25.
//

#ifndef SET_ANCHORED_POSITION_H
#define SET_ANCHORED_POSITION_H

#include <raylib.h>
#include "modules/engine/rendering/gui/components.h"

namespace rendering::gui::systems {
    inline void set_anchored_position_system(const Rectangle &rectangle, Anchor &anchor) {
        anchor.position.x = rectangle.x;
        anchor.position.y = rectangle.y;
    }
}
#endif //SET_ANCHORED_POSITION_H
