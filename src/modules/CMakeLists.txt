set(MODULES_SOURCES )
set(MODULES_HEADERS "base_module.h")

target_sources(${LIBRARY_NAME} PUBLIC
        ${MODULES_SOURCES}
        ${MODULES_HEADERS})

add_subdirectory("core")
add_subdirectory("physics")
add_subdirectory("rendering")