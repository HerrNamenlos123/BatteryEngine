
#include "Battery/pch.h"
#include "Battery/Log/Log.h"

namespace Battery {

	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;

	Log::Log() {

	}

	Log::~Log() {

	}

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("Battery");
		clientLogger = spdlog::stdout_color_mt("Client");

		SetLogLevel(BATTERY_DEFAULT_LOG_LEVEL);
	}

	void Log::SetLogLevel(spdlog::level::level_enum level) {
		coreLogger->set_level(level);
		clientLogger->set_level(level);
	}

}
