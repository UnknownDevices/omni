#pragma once

#pragma warning( push )
#pragma warning( disable : 4005 )

#include <spdlog/spdlog.h>

#pragma warning( pop )

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>
#include <omni/core.hpp>

#pragma warning( push )
#pragma warning( disable : 4251 )

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

        static void init();
        
        static void log(Logger::Level level, fmt::string_view msg);

    private:
        static inline std::shared_ptr<spdlog::logger> s_spdlog_logger;
    };

    template<typename TMsg>
    void log(Logger::Level level, const TMsg& msg)
    {
        Logger::log(level, msg);
    }

    template<typename TMsg>
    void debug_log(const TMsg& msg)
    {
        Logger::log(Logger::Level::Debug, msg);
    }

    template<typename TMsg>
    void trace_log(const TMsg& msg)
    {
        Logger::log(Logger::Level::Trace, msg);
    }

    template<typename TMsg>
    void info_log(const TMsg& msg)
    {
        Logger::log(Logger::Level::Info, msg);
    }

    template<typename TMsg>
    void warn_log(const TMsg& msg)
    {
        Logger::log(Logger::Level::Warn, msg);
    }

    template<typename TMsg>
    void error_log(const TMsg& msg)
    {
        Logger::log(Logger::Level::Error, msg);
    }

    template<typename TMsg>
    void critical_log(const TMsg& msg)
    {
        Logger::log(Logger::Level::Critical, msg);
    }

    template<typename TMsg>
    void soft_assert(std::string_view file, uint32_fast line, bool to_assert, const TMsg& msg)
    {
        if (to_assert)
            return;

        Logger::log(Logger::Level::Error,
            fmt::format("Soft assertion failed at: [{}][{}] with message: [{}].", file, line,
            msg));

    #if OMNI_BREAK_ON_FAILED_SOFT_ASSERTS
        __debugbreak();
    #endif
    }

    template<typename TMsg>
    void hard_assert(std::string_view file, uint32_fast line, bool to_assert, const TMsg& msg)
    {
        if (to_assert)
            return;

        Logger::log(Logger::Level::Error,
            fmt::format("Hard assertion failed at: [{}][{}] with message: [{}].", file, line, 
            msg));

    #if OMNI_BREAK_ON_FAILED_HARD_ASSERTS
        __debugbreak();
    #endif
    }

    template<typename ... TMsgArgs>
    void log(Logger::Level level, fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        Logger::log(level, fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...)));
    }

    template<typename ... TMsgArgs>
    void debug_log(fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        Logger::log(Logger::Level::Debug, 
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...)));
    }

    template<typename ... TMsgArgs>
    void trace_log(fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        Logger::log(Logger::Level::Trace, 
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...)));
    }

    template<typename ... TMsgArgs>
    void info_log(fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        Logger::log(Logger::Level::Info, 
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...)));
    }

    template<typename ... TMsgArgs>
    void warn_log(fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        Logger::log(Logger::Level::Warn, 
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...)));
    }

    template<typename ... TMsgArgs>
    void error_log(fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        Logger::log(Logger::Level::Error, 
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...)));
    }

    template<typename ... TMsgArgs>
    void critical_log(fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        Logger::log(Logger::Level::Critical, 
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...)));
    }

    template<typename ... TMsgArgs>
    void soft_assert(std::string_view file, uint32_fast line, bool to_assert,
        fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        if (to_assert)
            return;

        Logger::log(Logger::Level::Error,
            fmt::format("Soft assertion failed at: [{}][{}] with message: [{}].", file, line,
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...))));

    #if OMNI_BREAK_ON_FAILED_SOFT_ASSERTS
        __debugbreak();
    #endif
    }

    template<typename ... TMsgArgs>
    void hard_assert(std::string_view file, uint32_fast line, bool to_assert,
        fmt::format_string<TMsgArgs...> msg_fmt, TMsgArgs&& ... msg_args)
    {
        if (to_assert)
            return;

        Logger::log(Logger::Level::Error,
            fmt::format("Hard assertion failed at: [{}][{}] with message: [{}].", file, line,
            fmt::vformat(msg_fmt, fmt::make_format_args(msg_args...))));

    #if OMNI_BREAK_ON_FAILED_HARD_ASSERTS
        __debugbreak();
    #endif
    }
}

#define omni_soft_assert(...) Omni::soft_assert(__FILE__, __LINE__, __VA_ARGS__)
#define omni_hard_assert(...) Omni::hard_assert(__FILE__, __LINE__, __VA_ARGS__)

#define omni_assert_win32_call(to_assert, win32_func) omni_hard_assert(to_assert,                 \
    "Call to win32's " #win32_func " failed with message: [{}][{}]", GetLastError(),              \
    format_win32_error(GetLastError()))

#pragma warning( pop )
