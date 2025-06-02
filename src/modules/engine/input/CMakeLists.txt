set(INPUT_SOURCES "input_module.cpp")
set(INPUT_HEADERS "input_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${INPUT_SOURCES}
        ${INPUT_HEADERS})

add_subdirectory("systems")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)