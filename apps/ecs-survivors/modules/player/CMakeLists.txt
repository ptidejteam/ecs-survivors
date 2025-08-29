set(PLAYER_SOURCES "player_module.cpp")
set(PLAYER_HEADERS "player_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${PLAYER_SOURCES}
        ${PLAYER_HEADERS})

add_subdirectory("systems")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)