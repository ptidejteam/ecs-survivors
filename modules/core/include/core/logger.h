//
// Created by laurent on 9/12/25.
//

#ifndef CORE_LOGGER_H
#define CORE_LOGGER_H


#include <vector>
#include <string>
#include <functional>
#include <sstream>

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

        void log(LogLevel level, LogLocation location, const std::string& msg);

        static void rlLog(int loglevel, const char* txt, va_list args);

        const std::vector<std::string> get_log_history() const{ return log_history; }

        const char* get_level_string(LogLevel level) const;
        const char* get_location_string(LogLocation loc) const;
        const char* get_time_stamp() const;

        void AddSink(Sink sink);

    private:
        Logger() {
            AddSink([&](LogLevel level, LogLocation loc, std::string message) {
                std::stringstream s;
                s << Instance().get_time_stamp() << " " << Instance().get_level_string(level) << " "
                         << Instance().get_location_string(loc) << " " << message << std::endl;

                Instance().log_history.push_back(s.str());
           });
        };
        std::vector<Sink> mSinks;
        std::vector<std::string> log_history;
    };

} // namespace base

#define LOG_INFO(loc, msg) core::Logger::Instance().log(core::LogLevel::Info,loc,msg);
#define LOG_WARNING(loc, msg) core::Logger::Instance().log(core::LogLevel::Warning,loc,msg);
#define LOG_ERROR(loc, msg) core::Logger::Instance().log(core::LogLevel::Error,loc,msg);
#define LOG_DEBUG(loc, msg) core::Logger::Instance().log(core::LogLevel::Info,loc,msg);

#endif // CORE_LOGGER_H
