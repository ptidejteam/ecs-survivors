set(GAMEPLAY_SOURCES "gameplay_module.cpp")
set(GAMEPLAY_HEADERS "gameplay_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${GAMEPLAY_SOURCES}
        ${GAMEPLAY_HEADERS})

add_subdirectory("systems")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)