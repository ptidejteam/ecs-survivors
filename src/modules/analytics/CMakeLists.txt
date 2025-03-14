set(ANALYTICS_SOURCES "analytics_module.cpp")
set(ANALYTICS_HEADERS "analytics_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${ANALYTICS_SOURCES}
        ${ANALYTICS_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)