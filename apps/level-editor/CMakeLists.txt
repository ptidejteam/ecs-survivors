add_executable(level-editor "main.cpp")

add_dependencies(level-editor copy_to_build)

target_link_libraries(level-editor PUBLIC
        core
        input
        rendering
        gui
        tilemap
        ai
        gameplay
        debugging
        player
        raylib
        editor
        flecs::flecs_static)