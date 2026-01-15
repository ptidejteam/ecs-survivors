add_library(input
        src/input_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(input
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(input PUBLIC
        base
        core
        raylib #only using the datatypes
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(input PUBLIC cxx_std_20)