#pragma once

#define BATTERY_VERSION_FILE "version"	// No extension

//#define NETWORK_CERTIFICATE_FILE "cacert.pem"
#define BATTERY_DEFAULT_TITLE "BatteryEngine Application"

// Graphics
#define BATTERY_ANTIALIASING_LINE_FALLOFF 3
#define BATTERY_DEFAULT_BACKGROUND_COLOR glm::vec3(80, 80, 80)

// Enable Debug stuff
#ifdef _DEBUG
#define BATTERY_DEBUG
#endif

// Some logging
#define BATTERY_LOG_LEVEL_CRITICAL	spdlog::level::critical
#define BATTERY_LOG_LEVEL_ERROR		spdlog::level::err
#define BATTERY_LOG_LEVEL_WARN		spdlog::level::warn
#define BATTERY_LOG_LEVEL_INFO		spdlog::level::info
#define BATTERY_LOG_LEVEL_TRACE		spdlog::level::trace

#define BATTERY_LOG_LEVEL BATTERY_LOG_LEVEL_INFO