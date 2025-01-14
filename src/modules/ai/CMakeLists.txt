set(AI_SOURCES "ai_module.cpp")
set(AI_HEADERS "ai_module.h" "components.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${AI_SOURCES}
        ${AI_HEADERS})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static)