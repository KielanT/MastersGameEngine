#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <memory>

// Logger class used for logging to the console

namespace Engine
{
	class Logger
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> m_Logger;

	};
}

// Defines for logging to the console
// Only log in debug and release mode
#if defined E_DEBUG || defined E_RELEASE
#define LOG_INFO(...) ::Engine::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...) ::Engine::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_WARN(...) ::Engine::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Engine::Logger::GetLogger()->error(__VA_ARGS__)
#elif E_DISTRIBUTION
#define LOG_INFO(...) 
#define LOG_DEBUG(...)
#define LOG_WARN(...) 
#define LOG_ERROR(...) 
#endif // I_DEBUG