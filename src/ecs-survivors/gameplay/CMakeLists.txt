add_library(gameplay
        src/gameplay_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(gameplay
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(gameplay PUBLIC
        base
        core
        rendering
        physics
        input
        gui
        raylib

        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(gameplay PUBLIC cxx_std_20)