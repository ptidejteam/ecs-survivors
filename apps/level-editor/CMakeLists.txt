add_executable(level-editor "main.cpp")

add_dependencies(level-editor copy_to_build)

target_link_libraries(level-editor PUBLIC
        core
        input
        physics
        rendering
        gui
        tilemap
        ai
        debugging
        gameplay
        player
        raylib
        raygui
        editor
        flecs::flecs_static)