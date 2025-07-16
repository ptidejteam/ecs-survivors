//
// Created by Laurent Voisard on 12/22/2024.
//

#ifndef GUI_COMPONENTS_H
#define GUI_COMPONENTS_H
#include <raylib.h>
#include <string>
#include <flecs.h>
#include <functional>
#include <variant>

#include "modules/engine/core/components.h"

namespace rendering::gui {

    const int FONT_SIZE_16 = 16;
    const int FONT_SIZE_32 = 32;
    const int FONT_SIZE_48 = 48;
    const int FONT_SIZE_64 = 64;

    struct Button {
        Texture2D texture;
        Color normal_tint;
        Color selected_tint;
        Color hovered_tint;
        NPatchInfo info;
        // not sure this is the best thing to store

    };

    struct ButtonCallback {
        std::function<void()> callback;
    };

    struct ButtonLabel {
        std::string label;
        int font_size;
    };

    struct Text {
        std::string text;
        int font_size;
        int alignment;
    };

    struct FontAtlas {
        std::unordered_map<int, Font> fonts;
    };

    struct Outline {
        int border_size;
        Color border_color;
        Color fill_color;
    };

    struct Panel {
        Texture2D texture;
        NPatchInfo info;
    };

    struct MenuBar {
        int item_width;
        int border_size;
        Color border_color;
        Color fill_color;
    };

    struct MenuBarTab {
        std::string name;
        int item_spacing;
        bool active = false;
    };

    enum MenuBarTabItemType {
        TOGGLE,
        RUN
    };
    struct MenuBarTabItem {
        std::string name;
        flecs::system toggle_system_entity;
        MenuBarTabItemType type;
    };

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

    struct ProgressBar {
        float min_val;
        float max_val;
        float current_val;
    };

    struct WindowResizedEvent{};
}

#endif //GUI_COMPONENTS_H
