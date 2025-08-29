set(AI_SYSTEMS_HEADERS
        follow_target_system.h
        stop_when_arrived_at_target_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${AI_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

