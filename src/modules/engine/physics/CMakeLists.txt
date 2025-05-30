set(PHYSICS_SOURCES "physics_module.cpp")
set(PHYSICS_HEADERS "physics_module.h" "components.h" "pipeline_steps.h" "queries.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${PHYSICS_SOURCES}
        ${PHYSICS_HEADERS})

add_subdirectory("systems")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)
