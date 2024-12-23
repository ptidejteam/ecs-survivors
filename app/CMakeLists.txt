add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${LIBRARY_NAME}
        raylib
        flecs::flecs_static)