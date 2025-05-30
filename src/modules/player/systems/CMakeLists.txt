set(PLAYER_SYSTEMS_HEADERS
        scale_desired_velocity_system.h
        translate_horizontal_input_to_desired_vel_system.h
        translate_vertical_input_to_desired_vel_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${PLAYER_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

