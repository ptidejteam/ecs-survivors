set(AI_SYSTEMS_HEADERS)

target_sources(${LIBRARY_NAME} PUBLIC
        ${AI_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

