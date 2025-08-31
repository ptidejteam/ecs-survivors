add_executable(ecs-survivors "main.cpp"
        "game.cpp")

add_dependencies(ecs-survivors copy_to_build)

target_link_libraries(ecs-survivors PUBLIC
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
        flecs::flecs_static)