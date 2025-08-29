set(DEBUG_SYSTEMS_HEADERS
        debug_collidable_entities_system.h
        debug_closest_enemy_to_player_system.h
        debug_colliders_system.h
        debug_entity_count_system.h
        debug_fps_system.h
        debug_grid_system.h
        debug_mouse_position_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${DEBUG_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

