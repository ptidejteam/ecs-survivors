set(LIBRARY_SOURCES
        "game.cpp")
set(LIBRARY_HEADERS
        "game.h"
)
set(LIBRARY_INCLUDES
        "./")

add_library(${LIBRARY_NAME} STATIC
        ${LIBRARY_SOURCES}
        ${LIBRARY_HEADERS})

add_subdirectory("modules")

target_include_directories(${LIBRARY_NAME} PUBLIC
        ${LIBRARY_INCLUDES})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static
        tmxlite)

