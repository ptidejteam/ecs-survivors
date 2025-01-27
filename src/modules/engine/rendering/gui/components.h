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
        Rectangle rect;
        flecs::system onclick_system;
    };

    struct Text {
        std::string text;
        Rectangle rect;
    };

    struct CenteredTextDecorator {};

    struct OutlineDecorator {
    };

    struct CenteredDecorator {
    };
}

#endif //GUI_COMPONENTS_H
