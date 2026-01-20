//
// Created by laurent on 8/30/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include <iostream>


#include <flecs.h>
#include "logger.h"
namespace core {
    class Application {
    public:
        virtual ~Application() = default;

        virtual void init() {
            Logger::Instance().AddSink([](LogLevel level, LogLocation loc, const std::string& message) {
                std::cout << Logger::Instance().get_level_string(level) << " "
                          << Logger::Instance().get_location_string(loc) << " " << message << std::endl;
            });
        };
        virtual void run() = 0;

    protected:
        flecs::world m_world;
    };
} // namespace core
#endif // APPLICATION_H
