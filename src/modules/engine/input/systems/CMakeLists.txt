set(INPUT_SYSTEMS_HEADERS
        reset_horizontal_input_system.h
        reset_vertical_input_system.h
        set_horizontal_input_system.h
        set_vertical_input_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${INPUT_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

