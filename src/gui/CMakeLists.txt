add_library(gui
        src/gui_module.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(gui
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(gui PUBLIC
        base
        core
        rendering
        raylib #only using the datatypes
        raygui
        flecs::flecs_static
)

# Set C++ standard just in case
target_compile_features(gui PUBLIC cxx_std_20)