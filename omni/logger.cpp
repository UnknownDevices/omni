#include <omni/logger.hpp>

namespace Omni
{
   	std::shared_ptr<spdlog::logger> Logger::s_spdlog_logger;

	void 
	Logger::init()  
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_spdlog_logger = spdlog::stdout_color_mt("OMNI");
		s_spdlog_logger->set_level(spdlog::level::trace);

		log(Logger::Level::trace, "Logger::init returning succesfully...");
	}

	template<typename ... Args> void
    log(Logger::Level level, std::string_view fmt, Args&& ... args)
    {
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
        }
    }
}