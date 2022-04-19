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
		trace_log("Logger Initialized succesfully...");
	}
}
