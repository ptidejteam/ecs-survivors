add_library(base INTERFACE)

# Public headers (installed/consumed by others)
target_include_directories(base
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(base INTERFACE
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(base INTERFACE cxx_std_20)