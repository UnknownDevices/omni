#include <omni/logger.hpp>

namespace Omni
{
   	std::shared_ptr<spdlog::logger> Logger::m_spdlog_logger;

	void 
	Logger::init()  
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_spdlog_logger = spdlog::stdout_color_mt("OMNI");
		m_spdlog_logger->set_level(spdlog::level::trace);

		log(Logger::Level::trace, "Logger::init returning succesfully...");
	}

	template<typename ... Args> void
    log(Logger::Level level, std::string_view fmt, Args&& ... args)
    {
        switch(level)
        {
            case Logger::Level::debug:
                Logger::m_spdlog_logger->debug(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::trace:
                Logger::m_spdlog_logger->trace(fmt, std::forward<Args>(args)...);
                break;    
            case Logger::Level::info:
                Logger::m_spdlog_logger->info(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::warn:
                Logger::m_spdlog_logger->warn(fmt, std::forward<Args>(args)...);
                break;
            case Logger::Level::error:
                Logger::m_spdlog_logger->error(fmt, std::forward<Args>(args)...);
                break;
        }
    }
}