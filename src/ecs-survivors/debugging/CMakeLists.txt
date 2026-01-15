add_library(debugging
        src/debug_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(debugging
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(debugging PUBLIC
        base
        core
        input
        gui
        physics
        raylib
        imgui
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(debugging PUBLIC cxx_std_20)