#include <omni/logger.hpp>

namespace Omni
{
    void Logger::init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_spdlog_logger = spdlog::stdout_color_mt("OMNI");
        s_spdlog_logger->set_level(spdlog::level::trace);
        trace_log("Logger initialized successfully.");
    }

    void log(Logger::Level level, std::string_view msg)
    {
        switch (level)
        {
            
            case Logger::Level::Debug:
                Logger::s_spdlog_logger->debug(msg);
                break;
            case Logger::Level::Trace:
                Logger::s_spdlog_logger->trace(msg);
                break;
            case Logger::Level::Info:
                Logger::s_spdlog_logger->info(msg);
                break;
            case Logger::Level::Warn:
                Logger::s_spdlog_logger->warn(msg);
                break;
            case Logger::Level::Error:
                Logger::s_spdlog_logger->error(msg);
                break;
            case Logger::Level::Critical:
                Logger::s_spdlog_logger->critical(msg);
                break;
        }
    }
}
