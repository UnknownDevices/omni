#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>
#include <omni/core.hpp>

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
    log(Logger::Level level, std::string_view fmt, Args&& ... args) {
        switch(level)
        {
            case Logger::Level::debug:
                Logger::s_spdlog_logger->debug(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::trace:
                Logger::s_spdlog_logger->trace(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::info:
                Logger::s_spdlog_logger->info(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::warn:
                Logger::s_spdlog_logger->warn(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::error:
                Logger::s_spdlog_logger->error(fmt, std::forward<Args>(args)...);
                break;
			case Logger::Level::critical:
                Logger::s_spdlog_logger->critical(fmt, std::forward<Args>(args)...);
                break;
        }
    }

	template<typename ... Args> inline void
    debug_log(std::string_view fmt, Args&& ... args) {
		log(Logger::Level::debug, std::move(fmt), std::forward<Args>(args)...);
	}

	template<typename ... Args> inline void
    trace_log(std::string_view fmt, Args&& ... args) {
		log(Logger::Level::trace, std::move(fmt), std::forward<Args>(args)...);
	}

	template<typename ... Args> inline void
    info_log(std::string_view fmt, Args&& ... args) {
		log(Logger::Level::info, std::move(fmt), std::forward<Args>(args)...);
	}

	template<typename ... Args> inline void
    warn_log(std::string_view fmt, Args&& ... args) {
		log(Logger::Level::warn, std::move(fmt), std::forward<Args>(args)...);
	}

	template<typename ... Args> inline void
    error_log(std::string_view fmt, Args&& ... args) {
		log(Logger::Level::error, std::move(fmt), std::forward<Args>(args)...);
	}

	template<typename ... Args> inline void
    critical_log(std::string_view fmt, Args&& ... args) {
		log(Logger::Level::critical, std::move(fmt), std::forward<Args>(args)...);
	}
}
