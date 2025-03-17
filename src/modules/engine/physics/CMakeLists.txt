set(PHYSICS_SOURCES "physics_module.cpp")
set(PHYSICS_HEADERS "physics_module.h" "components.h" "spatial_hash_updating.h" "spatial_hash_rebuilding.h" "base_spatial_hash.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${PHYSICS_SOURCES}
        ${PHYSICS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)
