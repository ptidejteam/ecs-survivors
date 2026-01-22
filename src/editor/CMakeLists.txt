add_library(editor
        src/editor_module.cpp
        src/asset_browser.cpp
        src/imgui_utils.cpp
        src/item_views.cpp
)

# Public headers (installed/consumed by others)
target_include_directories(editor
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # during build
        $<INSTALL_INTERFACE:include>                            # when installed
)

# Link to dependencies
target_link_libraries(editor PUBLIC
        base
        core
        rendering
        raylib
        imgui
        rlimgui
        flecs::flecs_static
)



# Set C++ standard just in case
target_compile_features(editor PUBLIC cxx_std_20)