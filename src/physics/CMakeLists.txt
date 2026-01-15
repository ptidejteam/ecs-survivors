add_library(physics
        src/physics_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(physics
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(physics PUBLIC
        base
        core
        raylib #only using the datatypes
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(physics PUBLIC cxx_std_20)