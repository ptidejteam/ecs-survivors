set(GAMEPLAY_SYSTEMS_HEADERS
        add_chain_system.h
        add_multiproj_system.h
        add_pierce_system.h
        add_split_system.h
        deal_damage_on_collision_system.h
        decrement_chain_system.h
        decrement_multiproj_system.h
        decrement_pierce_system.h
        deal_damage_on_collision_system.h
        fire_projectile_system.h
        increment_chain_system.h
        increment_multiproj_system.h
        increment_pierce_system.h
        projectile_chain_collided_system.h
        projectile_no_effect_collided_system.h
        projectile_split_collision_system.h
        projectile_pierce_collided_system.h
        regen_health_system.h
        remove_chain_system.h
        remove_multproj_system.h
        remove_pierce_system.h
        remove_split_system.h
        restart_cooldown_system.h
        spawn_enemies_around_screen_system.h
        take_damage_system.h
        update_cooldown_system.h
)

target_sources(${LIBRARY_NAME} PUBLIC
        ${GAMEPLAY_SYSTEMS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

