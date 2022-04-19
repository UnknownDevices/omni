#pragma once

#include <omni/core.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace Omni
{
    class OMNI_API Logger
    {
    public:
        enum class Level
        {
            debug    = 0,
            trace    = 1,
            info     = 2,
            warn     = 3,
            error    = 4,
            critical = 5
        };

        template<typename ... Args> friend void
        log(Logger::Level level, std::string_view fmt, Args&& ... args);
 
        static void 
        init();

    private:
        static std::shared_ptr<spdlog::logger> s_spdlog_logger;
    };

    template<typename ... Args> void
    log(Logger::Level level, std::string_view fmt, Args&& ... args);

    template<typename ... Args> inline void
    debug_log(std::string_view fmt, Args&& ... args);

    template<typename ... Args> inline void
    trace_log(std::string_view fmt, Args&& ... args);

    template<typename ... Args> inline void
    info_log(std::string_view fmt, Args&& ... args);

    template<typename ... Args> inline void
    warn_log(std::string_view fmt, Args&& ... args);

    template<typename ... Args> inline void
    error_log(std::string_view fmt, Args&& ... args);

    template<typename ... Args> inline void
    critical_log(std::string_view fmt, Args&& ... args);
}
