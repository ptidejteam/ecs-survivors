add_executable(headless-app "main.cpp" "headless_app.cpp")

add_dependencies(headless-app copy_to_build)

target_link_libraries(headless-app PUBLIC
        core
        flecs::flecs_static)