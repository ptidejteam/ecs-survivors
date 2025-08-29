add_library(core
        src/core_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(core
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(core PUBLIC
        base
        raylib #only using the datatypes
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(core PUBLIC cxx_std_20)