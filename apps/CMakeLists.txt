add_executable(${PROJECT_NAME} "main.cpp")

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources
        COMMENT "Copying resources into binary directory")

add_dependencies(${PROJECT_NAME} copy_resources)

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${LIBRARY_NAME}
        raylib
        flecs::flecs_static)