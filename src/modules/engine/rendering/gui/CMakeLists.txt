set(GUI_LIBRARY_SOURCES "gui_module.cpp")
set(GUI_LIBRARY_HEADERS "components.h" "gui_module.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${GUI_LIBRARY_SOURCES}
        ${GUI_LIBRARY_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)