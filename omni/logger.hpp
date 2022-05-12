#pragma once

#pragma warning( push )
#pragma warning( disable : 4251 )

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>
#include <omni/core.hpp>

namespace Omni
{
    class OMNI_API Logger
    {
    public:
        enum class Level // TODO: Needs reworking
        {
            Debug    = 0,
            Trace    = 1,
            Info     = 2,
            Warn     = 3,
            Error    = 4,
            Critical = 5
        };

        friend void OMNI_API
            log(Logger::Level level, std::string_view fmt);

        static void
            init();

    private:
        static inline std::shared_ptr<spdlog::logger> s_spdlog_logger;
    };

    // TODO: Use FMT library instead
    template<typename ... Args>
    void log(Logger::Level level, std::string_view fmt, const Args& ... args)
    {
        log(level, std::format(fmt, args...));
    }

    template<typename ... Args>
    void debug_log(std::string_view fmt, const Args& ... args)
    {
        log(Logger::Level::Debug, std::format(fmt, args...));
    }

    template<typename ... Args>
    void trace_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Trace, std::format(fmt, args...));
    }

    template<typename ... Args>
    void info_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Info, std::format(fmt, args...));
    }

    template<typename ... Args>
    void warn_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Warn, std::format(fmt, args...));
    }

    template<typename ... Args>
    void error_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Error, std::format(fmt, args...));
    }

    template<typename ... Args>
    void critical_log(std::string_view fmt, Args&& ... args)
    {
        log(Logger::Level::Critical, std::format(fmt, args...));
    }

    template<typename ... Args>
    void soft_assert(bool to_assert, std::string_view fmt, Args&& ... args)
    {
        if (to_assert)
            return;

        log(Logger::Level::Warn, "Soft assertion failed with message: " + 
            std::format(fmt, args...));

    #if OMNI_BREAK_ON_FAILED_SOFT_ASSERTS
        __debugbreak();
    #endif
    }

    template<typename ... Args>
    void hard_assert(bool to_assert, std::string_view fmt, Args&& ... args)
    {
        if (to_assert)
            return;

        log(Logger::Level::Error, "Hard assertion failed with message: " + 
            std::format(fmt, args...));

    #if OMNI_BREAK_ON_FAILED_HARD_ASSERTS
        __debugbreak();
    #endif
    }
}

#pragma warning( pop )
 