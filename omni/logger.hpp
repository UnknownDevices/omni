#pragma once

#include <omni/core.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace Omni
{
    class OMNI_API Logger
    {
    // HACK: Refactor
    #if OMNI_DEBUG
    public:
        template<typename ... Args>
        static  auto error(fmt::format_string<Args...> fmt, Args &&...args) -> void
        {
            spdlog_logger->error(fmt, args...);
        }

        template<typename ... Args>
        static auto warn(fmt::format_string<Args...> fmt, Args &&...args) -> void
        {
            spdlog_logger->warn(fmt, args...);
        }

        template<typename ... Args>
        static  auto info(fmt::format_string<Args...> fmt, Args &&...args) -> void
        {
            spdlog_logger->info(fmt, args...);
        }

		static auto init() -> void
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");
	    	spdlog_logger = spdlog::stdout_color_mt("OMNI");
	    	spdlog_logger->set_level(spdlog::level::trace);
        }

        static std::shared_ptr<spdlog::logger> spdlog_logger;
    #else
    public:
        template<typename ... Args>
        static  auto error(fmt::format_string<Args...> fmt, Args &&...args) -> void;

        template<typename ... Args>
        static  auto warn(fmt::format_string<Args...> fmt, Args &&...args) -> void;

        template<typename ... Args>
        static  auto info(fmt::format_string<Args...> fmt, Args &&...args) -> void;

        template<typename ... Args>
        static  auto trace(fmt::format_string<Args...> fmt, Args &&...args) -> void;

		static auto init() -> void;
    #endif
    };
}