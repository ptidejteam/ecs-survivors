//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_COMPONENTS_H
#define GUI_COMPONENTS_H
#include <raylib.h>
#include <string>

#include "modules/engine/core/components.h"

namespace rendering::gui {


    struct Button {
        std::string text;
        flecs::system onclick_system;
    };

    struct Text {
        std::string text;
    };

    struct Decorator {
        flecs::system add_decoration_system;
        flecs::system remove_decoration_system;
    };

	struct Decorated {
		flecs::entity& entity;
	};

	struct Outline {
		int border_size;
		Color border_color;
		Color fill_color;
	};
	struct GuiElement{};
	struct Decoration {};
}

#endif //GUI_COMPONENTS_H
