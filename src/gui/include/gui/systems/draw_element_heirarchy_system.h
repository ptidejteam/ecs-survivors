//
// Created by laurent on 9/6/25.
//

#ifndef ECS_SURVIVORS_DRAW_ELEMENT_HEIRARCHY_SYSTEM_H
#define ECS_SURVIVORS_DRAW_ELEMENT_HEIRARCHY_SYSTEM_H
#include <flecs.h>
#include <raylib.h>
#include <gui/components.h>

#include "draw_outline_system.h"
#include "draw_progress_bar_system.h"
#include "draw_text_system.h"
#include "draw_textured_element_system.h"

namespace gui::systems {
    inline void draw_element_heirarchy_system(flecs::iter& it, size_t i, gui::GUIElement& element) {
        flecs::entity e = it.entity(i);
        // draw self, then draw children
        switch (element.type) {
            case TEXT:
                draw_text_system(e, e.get<Text>(), e.get<Rectangle>(), it.world().get<FontAtlas>());
                break;
            case OUTLINE:
                draw_outline_system(e.get<Rectangle>(), e.get<Outline>());
                break;
            case TEXTURED_ELEMENT:
                draw_textured_element_system(e.get_mut<TexturedElement>(), e.get<Rectangle>());
                break;
            case PROGRESS_BAR:
                draw_progress_bar_system(e.get<Rectangle>(), e.get<ProgressBar>());
                break;
            default:
                std::cout << "did not implement gui element" << std::endl;
        }

        it.world().query_builder<GUIElement>().with(flecs::ChildOf, e).build().each(draw_element_heirarchy_system);
    }
}
#endif // ECS_SURVIVORS_DRAW_ELEMENT_HEIRARCHY_SYSTEM_H
