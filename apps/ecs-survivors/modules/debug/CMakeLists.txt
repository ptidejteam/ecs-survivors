set(DEBUG_SOURCES "debug_module.cpp")
set(DEBUG_HEADERS "debug_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${DEBUG_SOURCES}
        ${DEBUG_HEADERS})

add_subdirectory("systems")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)