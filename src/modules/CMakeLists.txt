set(MODULES_SOURCES )
set(MODULES_HEADERS "base_module.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${MODULES_SOURCES}
        ${MODULES_HEADERS})

add_subdirectory("engine")
add_subdirectory("player")
add_subdirectory("ai")
add_subdirectory("tilemap")
add_subdirectory("gameplay")
add_subdirectory("debug")
