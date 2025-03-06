//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_COMPONENTS_H
#define GUI_COMPONENTS_H
#include <raylib.h>
#include <string>
#include <flecs.h>
#include "modules/engine/core/components.h"

namespace rendering::gui {

    enum HORIZONTAL_ANCHOR {
        LEFT,
        CENTER,
        RIGHT
    };

    enum VERTICAL_ANCHOR {
        TOP,
        MIDDLE,
        BOTTOM
    };

    struct Button {
        std::string text;
        flecs::system on_click_system;
    };

    struct Text {
        std::string text;
        int alignment;
    };

	struct Outline {
		int border_size;
		Color border_color;
		Color fill_color;
	};

    struct Anchor {
        Vector2 position;
        HORIZONTAL_ANCHOR horizontal_anchor;
        VERTICAL_ANCHOR vertical_anchor;

        Anchor() {
            position = {};
            this->horizontal_anchor = HORIZONTAL_ANCHOR::LEFT;
            this->vertical_anchor = VERTICAL_ANCHOR::TOP;
        };

        Anchor(HORIZONTAL_ANCHOR horizontal_anchor, VERTICAL_ANCHOR vertical_anchor) {
            position = {};
            this->horizontal_anchor = horizontal_anchor;
            this->vertical_anchor = vertical_anchor;
        };
    };

    struct Panel {
        std::string name;
    };
}

#endif //GUI_COMPONENTS_H
