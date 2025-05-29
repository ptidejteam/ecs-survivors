set(PHYSICS_SYSTEMS_HEADERS
        collision_cleanup_system.h
        collision_detection_system.h
        collision_resolution_system.h
        reset_desired_velocity_system.h
        update_position_system.h
        update_velocity_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${PHYSICS_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

