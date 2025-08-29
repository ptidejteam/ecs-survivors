add_executable(ecs-survivors "main.cpp"
        "ecs-survivors/game.cpp"
        "ecs-survivors/modules/gameplay/gameplay_module.cpp"
        "ecs-survivors/modules/debug/debug_module.cpp"
        "ecs-survivors/modules/player/player_module.cpp"
        "ecs-survivors/modules/ai/ai_module.cpp")

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        ${PROJECT_BINARY_DIR}/assets
        COMMENT "Copying resources into binary directory")

add_dependencies(ecs-survivors copy_resources)

target_link_libraries(ecs-survivors PUBLIC
        core
        input
        physics
        rendering
        gui_game
        tilemap
        raylib
        raygui
        flecs::flecs_static)