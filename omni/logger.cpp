#include <omni/logger.hpp>

namespace Omni
{
void Logger::init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    spdlog_logger_ = spdlog::stdout_color_mt("OMNI");
    spdlog_logger_->set_level(spdlog::level::trace);
    trace_log("Logger initialized successfully.");
}

void Logger::log(Logger::Level level, fmt::string_view msg)
{
    switch (level)
    {
        case Logger::Level::Debug:
            Logger::spdlog_logger_->debug(msg);
            break;
        case Logger::Level::Trace:
            Logger::spdlog_logger_->trace(msg);
            break;
        case Logger::Level::Info:
            Logger::spdlog_logger_->info(msg);
            break;
        case Logger::Level::Warn:
            Logger::spdlog_logger_->warn(msg);
            break;
        case Logger::Level::Error:
            Logger::spdlog_logger_->error(msg);
            break;
        case Logger::Level::Critical:
            Logger::spdlog_logger_->critical(msg);
            break;
    }
}
}
