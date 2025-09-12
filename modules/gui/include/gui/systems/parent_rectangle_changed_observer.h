//
// Created by laurent on 29/05/25.
//

#ifndef PARENT_RECTANGLE_CHANGED_OBSERVER_H
#define PARENT_RECTANGLE_CHANGED_OBSERVER_H

#include <flecs.h>
#include <raylib.h>

#include "gui/components.h"

namespace gui::systems {
    inline void on_parent_rectangle_changed_observer(flecs::entity e, const Rectangle &parent, Rectangle &rec, const Anchor& anchor) {
        switch (anchor.horizontal_anchor) {
            case CENTER:
                rec.x = (int)(anchor.position.x + parent.x + parent.width / 2);
                break;
            case RIGHT:
                rec.x = (int)(anchor.position.x + parent.x + parent.width);
                break;
            default:
                rec.x = (int)(anchor.position.x + parent.x);
                break;
        }
        switch (anchor.vertical_anchor) {
            case MIDDLE:
                rec.y = (int)(anchor.position.y + parent.y + parent.height / 2);
                break;
            case BOTTOM:
                rec.y = (int)(anchor.position.y + parent.y + parent.height);
                break;
            default:
                rec.y = (int)(anchor.position.y + parent.y);
                break;
        }


    }
}

#endif //PARENT_RECTANGLE_CHANGED_OBSERVER_H
