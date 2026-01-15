add_library(player
        src/player_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(player
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(player PUBLIC
        base
        core
        input
        physics
        raylib
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(player PUBLIC cxx_std_20)