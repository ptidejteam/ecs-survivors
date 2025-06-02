set(CORE_SYSTEMS_HEADERS
        destroy_entity_after_time_system.h
        destroy_entity_after_frame_system.h
        remove_empty_tables_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${CORE_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

