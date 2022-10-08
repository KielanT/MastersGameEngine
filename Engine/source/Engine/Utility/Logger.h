#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <memory>

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