add_executable(headless "main.cpp" "headless_app.cpp")

add_dependencies(headless copy_to_build)

target_link_libraries(headless PUBLIC
        core
        input
        physics
        tilemap
        ai
        debugging
        gameplay
        player
        flecs::flecs_static)