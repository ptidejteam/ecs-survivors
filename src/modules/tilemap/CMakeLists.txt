set(TILEMAP_SOURCES "tilemap_module.cpp")
set(TILEMAP_HEADERS "tilemap_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${TILEMAP_SOURCES}
        ${TILEMAP_HEADERS})

add_subdirectory("systems")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static
        tmxlite)