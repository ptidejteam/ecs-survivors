set(GUI_SYSTEMS_HEADERS
        load_style_system.h
        parent_rectangle_changed_observer.h
        set_anchored_position_system.h
        set_gui_canvas_size_system.h
        check_window_resized_system.h
        draw_button_system.h
        draw_menu_bar_system.h
        draw_menu_bar_tab_system.h
        draw_menu_bar_tab_item_system.h
        draw_outline_system.h
        draw_button_system.h
        draw_panel_system.h
        draw_text_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${GUI_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

