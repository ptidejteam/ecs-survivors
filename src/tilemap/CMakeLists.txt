add_library(tilemap
        src/tilemap_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(tilemap
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(tilemap PUBLIC
        base
        core
        physics
        rendering
        raylib #only using the datatypes
        tmxlite
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(tilemap PUBLIC cxx_std_20)