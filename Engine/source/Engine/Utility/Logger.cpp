#include "epch.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Engine
{
	std::shared_ptr<spdlog::logger> Logger::m_Logger;

	void Logger::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Indigo.log", true));

		logSinks[0]->set_pattern("%^[%T] [%l]: %v%$");
		logSinks[1]->set_pattern("[%T] [%l]: %v");


		m_Logger = std::make_shared<spdlog::logger>("Engine", begin(logSinks), end(logSinks));
		spdlog::register_logger(m_Logger);
		m_Logger->set_level(spdlog::level::trace);
		m_Logger->flush_on(spdlog::level::trace);
	}
}