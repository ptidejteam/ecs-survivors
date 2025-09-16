//
// Created by laurent on 9/12/25.
//

#ifndef CORE_LOGGER_H
#define CORE_LOGGER_H


#include <functional>
#include <sstream>
#include <string>
#include <vector>

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
        using Sink = std::function<void(LogLevel, LogLocation, const std::string &)>;

        static Logger &Instance();

        void log(LogLevel level, LogLocation location, const std::string &msg);

        static void rlLog(int loglevel, const char *txt, va_list args);


        const std::vector<std::string> get_log_history() const { return log_history; }

        const char *get_level_string(LogLevel level) const;
        const char *get_location_string(LogLocation loc) const;
        const char *get_time_stamp() const;

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

} // namespace core

template<typename... Args>
std::string concat_str(Args &&...args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

#define LOG_INFO(loc, ...) core::Logger::Instance().log(core::LogLevel::Info,loc,concat_str(__VA_ARGS__));
#define LOG_WARNING(loc, ...) core::Logger::Instance().log(core::LogLevel::Warning,loc,concat_str(__VA_ARGS__));
#define LOG_ERROR(loc, ...) core::Logger::Instance().log(core::LogLevel::Error,loc,concat_str(__VA_ARGS__));
#define LOG_DEBUG(loc, ...) core::Logger::Instance().log(core::LogLevel::Info,loc,concat_str(__VA_ARGS__));

#endif // CORE_LOGGER_H
