set(TILEMAP_SYSTEMS_HEADERS
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${TILEMAP_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static
        tmxlite)
