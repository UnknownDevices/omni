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
            Debug    = 0,
            Trace    = 1,
            Info     = 2,
            Warn     = 3,
            Error    = 4,
            Critical = 5
        };

        template<typename ... Args> friend void
            log(Logger::Level level, std::string_view fmt, Args&& ... args);

        static void
            init();

    private:
        static inline std::shared_ptr<spdlog::logger> s_spdlog_logger;
    };

    template<typename ... Args> void
        log(Logger::Level level, std::string_view fmt, Args&& ... args)
    {
        switch (level)
        {
            case Logger::Level::Debug:
                Logger::s_spdlog_logger->debug(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::Trace:
                Logger::s_spdlog_logger->trace(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::Info:
                Logger::s_spdlog_logger->info(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::Warn:
                Logger::s_spdlog_logger->warn(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::Error:
                Logger::s_spdlog_logger->error(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::Critical:
                Logger::s_spdlog_logger->critical(fmt, std::forward<Args>(args)...);
                break;
        }
    }

    template<typename ... Args> OMNI_INLINE void
        debug_log(std::string_view fmt, const Args& ... args)
    {
        log(Logger::Level::Debug, std::move(fmt), args...);
    }

    template<typename ... Args> OMNI_INLINE void
        trace_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Trace, std::move(fmt), std::forward<Args>(args)...);
    }

    template<typename ... Args> OMNI_INLINE void
        info_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Info, std::move(fmt), std::forward<Args>(args)...);
    }

    template<typename ... Args> OMNI_INLINE void
        warn_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Warn, std::move(fmt), std::forward<Args>(args)...);
    }

    template<typename ... Args> OMNI_INLINE void
        error_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Error, std::move(fmt), std::forward<Args>(args)...);
    }

    template<typename ... Args> OMNI_INLINE void
        critical_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Critical, std::move(fmt), std::forward<Args>(args)...);
    }

    template<typename ... Args> OMNI_INLINE void
        soft_assert(bool to_assert, std::string_view fmt, Args&& ... args)
    {
        if (to_assert)
            return;

        log(Logger::Level::Warn, std::string("Soft assertion failed with message: ").append(fmt),
            std::forward<Args>(args)...);

        #if OMNI_BREAK_ON_FAILED_SOFT_ASSERTS
        __debugbreak();
        #endif
    }

    template<typename ... Args> OMNI_INLINE void
        hard_assert(bool to_assert, std::string_view fmt, Args&& ... args)
    {
        if (to_assert)
            return;

        log(Logger::Level::Error, std::string("Hard assertion failed with message: ").append(fmt),
            std::forward<Args>(args)...);

        #if OMNI_BREAK_ON_FAILED_HARD_ASSERTS
        __debugbreak();
        #endif
    }
}
