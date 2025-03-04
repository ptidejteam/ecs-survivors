//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_COMPONENTS_H
#define GUI_COMPONENTS_H
#include <raylib.h>
#include <string>

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
        flecs::system onclick_system;
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
        HORIZONTAL_ANCHOR horizontal_anchor;
        VERTICAL_ANCHOR vertical_anchor;
    };

    struct Panel {

    };
}

#endif //GUI_COMPONENTS_H
