#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Battery/Core/Config.h"

namespace Battery {

	class Log {
	public:
		Log();
		~Log();

		static void Init();

		static void SetLogLevel(spdlog::level::level_enum level);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; };

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};

}

#ifdef BATTERY_DEBUG
#define LOG_INIT()				Battery::Log::Init();
#define LOG_CORE_TRACE(...)		Battery::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_WARN(...)		Battery::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_INFO(...)		Battery::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_ERROR(...)		Battery::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...)	Battery::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LOG_TRACE(...)		Battery::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_WARN(...)		Battery::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)		Battery::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_ERROR(...)		Battery::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	Battery::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LOG_SET_LOGLEVEL(...)	Battery::Log::SetLogLevel(__VA_ARGS__)
#else
#define LOG_INIT()				
#define LOG_CORE_TRACE(...)		
#define LOG_CORE_WARN(...)		
#define LOG_CORE_INFO(...)		
#define LOG_CORE_ERROR(...)		
#define LOG_CORE_CRITICAL(...)	
#define LOG_TRACE(...)	
#define LOG_WARN(...)	
#define LOG_INFO(...)	
#define LOG_ERROR(...)	
#define LOG_CRITICAL(...)
#define LOG_SET_LOGLEVEL(...)
#endif
