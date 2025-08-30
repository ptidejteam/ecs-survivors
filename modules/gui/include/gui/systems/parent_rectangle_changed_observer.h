//
// Created by laurent on 29/05/25.
//

#ifndef PARENT_RECTANGLE_CHANGED_OBSERVER_H
#define PARENT_RECTANGLE_CHANGED_OBSERVER_H

#include <flecs.h>
#include <raylib.h>

#include "gui/components.h"

namespace gui::systems {
    inline void on_parent_rectangle_changed_observer(flecs::entity e, const Rectangle &parent) {
        if (!e.has<Anchor>()) return;
        if (!e.has<Rectangle>()) return;

        auto anchor = e.get<Anchor>();

        Rectangle temp{e.get<Rectangle>()};
        switch (anchor.horizontal_anchor) {
            case CENTER:
                temp.x = anchor.position.x + parent.x + parent.width / 2;
                break;
            case RIGHT:
                temp.x = anchor.position.x + parent.x + parent.width;
                break;
            default:
                temp.x = anchor.position.x + parent.x;
                break;
        }
        switch (anchor.vertical_anchor) {
            case MIDDLE:
                temp.y = anchor.position.y + parent.y + parent.height / 2;
                break;
            case BOTTOM:
                temp.y = anchor.position.y + parent.y + parent.height;
                break;
            default:
                temp.y = anchor.position.y + parent.y;
                break;
        }
        e.set<Rectangle>({temp});
    }
}

#endif //PARENT_RECTANGLE_CHANGED_OBSERVER_H
