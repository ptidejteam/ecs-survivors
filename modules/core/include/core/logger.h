//
// Created by laurent on 9/12/25.
//

#ifndef CORE_LOGGER_H
#define CORE_LOGGER_H


#include <vector>
#include <string>
#include <functional>
namespace core {

    enum LogLevel {
        Info,
        Warning,
        Error,
        Debug,
    };

    enum LogLocation {
        Base,
        Core,
        Ai,
        Gameplay,
        Debugging,
        Player,
        Editor,
        Gui,
        Input,
        Physics,
        Rendering,
        Tilemap,
        Raylib
    };

    class Logger {
    public:
        using Sink = std::function<void(LogLevel, LogLocation, const std::string&)>;

        static Logger& Instance();

        void Log(LogLevel level, LogLocation location, const std::string& msg);

        static void rlLog(int loglevel, const char* txt, va_list args);

        const char* get_level_string(LogLevel level) const;
        const char* get_location_string(LogLocation loc) const;

        void AddSink(Sink sink);

    private:
        Logger() = default;
        std::vector<Sink> mSinks;
    };

} // namespace base

#define LOG_INFO(loc, msg) core::Logger::Instance().Log(core::LogLevel::Info,loc,msg);
#define LOG_WARNING(loc, msg) core::Logger::Instance().Log(core::LogLevel::Warning,loc,msg);
#define LOG_ERROR(loc, msg) core::Logger::Instance().Log(core::LogLevel::Error,loc,msg);
#define LOG_DEBUG(loc, msg) core::Logger::Instance().Log(core::LogLevel::Info,loc,msg);

#endif // CORE_LOGGER_H
