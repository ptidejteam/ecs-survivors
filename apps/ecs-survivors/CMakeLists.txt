add_executable(ecs-survivors "main.cpp"
        "game.cpp"
        "modules/gameplay/gameplay_module.cpp"
        "modules/debug/debug_module.cpp"
        "modules/player/player_module.cpp"
        "modules/ai/ai_module.cpp")

add_custom_target(copy_to_survivors ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        ${PROJECT_BINARY_DIR}/apps/ecs-survivors/assets
        COMMENT "Copying resources into binary directory")

add_dependencies(ecs-survivors copy_to_survivors)

target_link_libraries(ecs-survivors PUBLIC
        core
        input
        physics
        rendering
        gui
        tilemap
        raylib
        raygui
        flecs::flecs_static)