cmake_minimum_required(VERSION 3.28)
project(ECS_Survivors VERSION 1.0.0 LANGUAGES C CXX)

set(EXECUTABLE_NAME Executable)
set(LIBRARY_NAME Library)

set(CMAKE_CXX_STANDARD 20)
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Web Configurations
if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS  "\
        ${CMAKE_EXE_LINKER_FLAGS} \
        --embed-file resources \
        -s USE_GLFW=3 \
        -s ASSERTIONS=1 \
        -s WASM=1 \
        -s ASYNCIFY \
        -s GL_ENABLE_GET_PROC_ADDRESS=1 \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s STACK_SIZE=67108864 \
        -s EXPORTED_RUNTIME_METHODS=cwrap")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif ()

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

set(FLECS_VERSION 4.0.5)
find_package(flecs ${FLECS_VERSION} QUIET) # QUIET or REQUIRED
if (NOT flecs_FOUND) # If there's none, fetch and build flecs
    include(FetchContent)
    FetchContent_Declare(
            flecs
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/SanderMertens/flecs/archive/refs/tags/v${FLECS_VERSION}.tar.gz
    )
    FetchContent_GetProperties(flecs)
    if (NOT flecs_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(flecs)
    endif()
endif()


link_directories("./libs")

# Our Project
add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("app")


target_link_libraries(${LIBRARY_NAME} PUBLIC
        raylib
        flecs::flecs_static
)

