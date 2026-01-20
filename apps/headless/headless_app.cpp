//
// Created by laurent on 1/20/26.
//

#include "headless_app.h"

#include "core/core_module.h"
void HeadlessApp::init() {
    Application::init();
}
void HeadlessApp::run() {

    m_world = flecs::world();

    m_world.import<core::CoreModule>();

    m_world.set_target_fps(100);

    m_world.system()
    .run([](flecs::iter& it) {
        LOG_DEBUG(core::LogLocation::App, std::format("Headless App running at {}", 1.0f / it.delta_time()));
    });


    while (m_world.progress()) {

    }
}

