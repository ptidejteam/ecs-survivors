//
// Created by laurent on 9/12/25.
//

#ifndef CORE_GRAPHICAL_APPLICATION_H
#define CORE_GRAPHICAL_APPLICATION_H
#include "application.h"
#include "raylib.h"

namespace core {
    class GraphicalApplication: public Application {
    public:
        GraphicalApplication(const char* window_name, int window_width, int window_height): m_window_name(window_name), m_window_width(window_width), m_window_height(window_height){};

        void init() override {

            Application::init();

            SetTraceLogCallback(Logger::rlLog);
            SetTraceLogLevel(LOG_WARNING);
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
            InitWindow(m_window_width, m_window_height, m_window_name.c_str());
            SetExitKey(KEY_NULL);
        };
        protected:
            std::string m_window_name;
            int m_window_width;
            int m_window_height;
    };

}
#endif // CORE_GRAPHICAL_APPLICATION_H
