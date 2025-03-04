//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include <raygui.h>
#include <raylib.h>

#include "components.h"
#include "modules/base_module.h"
#include "flecs.h"
#include "modules/engine/core/core_module.h"

namespace rendering::gui {
    class GUIModule : public BaseModule<GUIModule> {
    public:
        // do not add implementation to the constructor
        GUIModule(flecs::world &world): BaseModule(world) {
        };

        static auto update_gui_anchored_position() {
            return [](flecs::entity e) {
                if (!e.has<Anchor>()) return;
                if (!e.has<Rectangle>()) return;
                if (!e.parent().is_alive()) return;
                auto parent = e.parent().get<Rectangle>();

                auto anchor = e.get<Anchor>();

                Rectangle temp{*e.get<Rectangle>()};

                switch (anchor->horizontal_anchor) {
                    case HORIZONTAL_ANCHOR::CENTER:
                        temp.x = anchor->rectangle.x + parent->x + parent->width / 2;
                        break;
                    case HORIZONTAL_ANCHOR::RIGHT:
                        temp.x = anchor->rectangle.x + parent->x + parent->width;
                        break;
                    default:
                        temp.x = anchor->rectangle.x + parent->x;
                        break;
                }
                switch (anchor->vertical_anchor) {
                    case VERTICAL_ANCHOR::MIDDLE:
                        temp.y = anchor->rectangle.y + parent->y + parent->height / 2;
                        break;
                    case VERTICAL_ANCHOR::BOTTOM:
                        temp.y = anchor->rectangle.y + parent->y + parent->height;
                        break;
                    default:
                        temp.y = anchor->rectangle.y + parent->y;
                        break;
                }
                e.set<Rectangle>({temp});
                e.children([&, parent](flecs::entity child) {
                    child.emit<core::WindowResizedEvent>();
                });
            };
        }

    private:
        void register_components(flecs::world &world);

        void register_systems(flecs::world &world);

        static Color font_color() { return GetColor(GuiGetStyle(TEXTBOX, TEXT_COLOR_NORMAL)); }


        friend class BaseModule<GUIModule>;
    };
}

#endif //GUI_MODULE_H
