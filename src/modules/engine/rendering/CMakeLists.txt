set(RENDERING_LIBRARY_SOURCES "rendering_module.cpp")
set(RENDERING_LIBRARY_HEADERS "rendering_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${RENDERING_LIBRARY_SOURCES}
        ${RENDERING_LIBRARY_HEADERS})

add_subdirectory("systems")
add_subdirectory("gui")

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)