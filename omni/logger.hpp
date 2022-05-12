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
            log(Logger::Level level, std::string_view msg);

        static void
            init();

    private:
        static inline std::shared_ptr<spdlog::logger> s_spdlog_logger;
    };

    // TODO: Use FMT library instead
    template<typename ... MsgArgs>
    void log(Logger::Level level, std::string_view msg_fmt, const MsgArgs& ... msg_args)
    {
        log(level, std::format(msg_fmt, msg_args...));
    }

    template<typename ... MsgArgs>
    void debug_log(std::string_view msg_fmt, const MsgArgs& ... msg_args)
    {
        log(Logger::Level::Debug, std::format(msg_fmt, msg_args...));
    }

    template<typename ... MsgArgs>
    void trace_log(std::string_view msg_fmt, MsgArgs&& ... msg_args)
    {
        log(Logger::Level::Trace, std::format(msg_fmt, msg_args...));
    }

    template<typename ... MsgArgs>
    void info_log(std::string_view msg_fmt, MsgArgs&& ... msg_args)
    {
        log(Logger::Level::Info, std::format(msg_fmt, msg_args...));
    }

    template<typename ... MsgArgs>
    void warn_log(std::string_view msg_fmt, MsgArgs&& ... msg_args)
    {
        log(Logger::Level::Warn, std::format(msg_fmt, msg_args...));
    }

    template<typename ... MsgArgs>
    void error_log(std::string_view msg_fmt, MsgArgs&& ... msg_args)
    {
        log(Logger::Level::Error, std::format(msg_fmt, msg_args...));
    }

    template<typename ... MsgArgs>
    void critical_log(std::string_view msg_fmt, MsgArgs&& ... msg_args)
    {
        log(Logger::Level::Critical, std::format(msg_fmt, msg_args...));
    }

    template<typename ... MsgArgs>
    void soft_assert(bool to_assert, std::string_view msg_fmt, MsgArgs&& ... msg_args)
    {
        if (to_assert)
            return;

        log(Logger::Level::Error,
            std::format("Soft assertion failed at: [{}, {}] with message: [{}].", 
            file, line, std::format(msg_fmt, msg_msg_args...)));

    #if OMNI_BREAK_ON_FAILED_SOFT_ASSERTS   
        __debugbreak();
    #endif
    }

    template<typename ... MsgArgs> 
    void hard_assert(std::string_view file, uint32_fast line, bool to_assert,
        std::string_view msg_fmt, MsgArgs&& ... msg_msg_args)
    {
        if (to_assert)
            return;

        log(Logger::Level::Error,
            std::format("Hard assertion failed at: [{}, {}] with message: [{}].", 
            file, line, std::format(msg_fmt, msg_msg_args...)));

    #if OMNI_BREAK_ON_FAILED_HARD_ASSERTS
        __debugbreak();
    #endif
    }

}

#define omni_soft_assert(...) Omni::soft_assert(__FILE__, __LINE__, __VA_ARGS__)
#define omni_hard_assert(...) Omni::hard_assert(__FILE__, __LINE__, __VA_ARGS__)

#pragma warning( pop )
 