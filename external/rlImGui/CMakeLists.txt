add_library(rlimgui STATIC
        rlImGui.cpp
)

target_include_directories(rlimgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
)

target_link_libraries(rlimgui PUBLIC imgui raylib)