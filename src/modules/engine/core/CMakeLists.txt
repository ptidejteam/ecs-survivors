set(CORE_SOURCES "core_module.cpp")
set(CORE_HEADERS "core_module.h" "components.h" "queries.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${CORE_SOURCES}
        ${CORE_HEADERS})

add_subdirectory("systems")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)

