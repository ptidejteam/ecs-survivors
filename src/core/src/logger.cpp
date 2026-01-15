//
// Created by laurent on 9/12/25.
//

#include "core/logger.h"

#include <iostream>
#include <ctime>
#include <iomanip>

namespace core {

    Logger &Logger::Instance() {
        static Logger instance;
        return instance;
    }

    void Logger::log(LogLevel level, LogLocation loc, const std::string &msg) {
        for (auto &sink: mSinks) {
            sink(level, loc, msg);
        }
    }
    // Show trace log messages (LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG)
    void Logger::rlLog(int msgType, const char *txt, va_list args) {
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), txt, args);

        LogLevel level = LogLevel::Info;
        if (msgType == 4)
            level = LogLevel::Warning;
        else if (msgType == 5)
            level = LogLevel::Error;
        else if (msgType == 2)
            level = LogLevel::Debug;

        Instance().log(level, LogLocation::Raylib, buffer);
    }
    const char *Logger::get_level_string(LogLevel level) const {
        switch (level) {
            case LogLevel::Info:
                return "[INFO]";
            case LogLevel::Warning:
                return "[WARN]";
            case LogLevel::Error:
                return "[ERROR]";
        }
    }
    const char *Logger::get_location_string(LogLocation loc) const {
        switch (loc) {
            case LogLocation::Base:
                return "[BASE]";
            case LogLocation::Core:
                return "[CORE]";
            case LogLocation::Editor:
                return "[EDITOR]";
            case LogLocation::Gui:
                return "[GUI]";
            case LogLocation::Rendering:
                return "[RENDERING]";
            case LogLocation::Input:
                return "[INPUT]";
            case LogLocation::Physics:
                return "[PHYSICS]";
            case LogLocation::Tilemap:
                return "[TILEMAP]";
            case LogLocation::Debugging:
                return "[DEBUGGING]";
            case LogLocation::Gameplay:
                return "[GAMEPLAY]";
            case LogLocation::Player:
                return "[PLAYER]";
            case LogLocation::Ai:
                return "[AI]";
            case LogLocation::Raylib:
                return "[RAYLIB]";
        }
    }

    const char * Logger::get_time_stamp() const {
        time_t timestamp;
        time(&timestamp);
        std::ostringstream oss;
        oss << "[" << std::put_time(std::localtime(&timestamp), "%H:%M:%S") << "]";
        return oss.str().c_str();
    }

    void Logger::AddSink(Sink sink) { mSinks.push_back(std::move(sink)); }

} // namespace base
