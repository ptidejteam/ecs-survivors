add_executable(level-editor "main.cpp")

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        ${PROJECT_BINARY_DIR}/apps/level-editor/assets
        COMMENT "Copying resources into binary directory")

add_dependencies(level-editor copy_resources)


target_link_libraries(level-editor PUBLIC
        core
        input
        rendering
        gui
        editor
        tilemap
        raylib
        flecs::flecs_static)