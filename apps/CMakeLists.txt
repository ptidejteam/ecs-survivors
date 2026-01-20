add_custom_target(copy_to_build ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        ${PROJECT_BINARY_DIR}/assets
        COMMENT "Copying resources into binary directory")

add_subdirectory("ecs-survivors")
add_subdirectory("level-editor")
add_subdirectory("headless")