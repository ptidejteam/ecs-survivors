add_library(ai
        src/ai_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(ai
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(ai PUBLIC
        base
        core
        physics
        raylib
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(ai PUBLIC cxx_std_20)