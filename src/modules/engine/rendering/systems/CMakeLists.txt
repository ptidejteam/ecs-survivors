set(RENDERING_SYSTEMS_HEADERS
        begin_drawing_system.h
        determine_visible_entities_system.h
        draw_entity_with_texture_system.h
        draw_health_bar_system.h
        end_drawing_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${RENDERING_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

