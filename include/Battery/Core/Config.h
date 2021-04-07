#pragma once

// Enable Debug stuff
#ifdef _DEBUG
#define BATTERY_DEBUG
#define BATTERY_PROFILING
#endif

// Enable experimental features
#define ALLEGRO_UNSTABLE

//#define BATTERY_ENABLE_ALLOCATION_TRACKING
//#define BATTERY_ENABLE_ALLOCATION_PRINTS

#define BATTERY_VERSION_FILE "version"	// No extension

//#define NETWORK_CERTIFICATE_FILE "cacert.pem"
#define BATTERY_DEFAULT_TITLE "BatteryEngine Window"
#define BATTERY_DEFAULT_FOLDER_NAME "BatteryApplication"

// Profiling
#define BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER 64
#define BATTERY_PROFILING_MAX_SCOPED_NUMBER 64
#define BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH 64
#define BATTERY_PROFILING_SCOPED_STRING_LENGTH 64

// Graphics
#define BATTERY_PROFILING_FONT "C:/Windows/Fonts/consola.ttf"
#define BATTERY_PROFILING_FONT_SIZE 14
#define BATTERY_MIN_WINDOW_WIDTH 200
#define BATTERY_MIN_WINDOW_HEIGHT 200
#define BATTERY_ANTIALIASING_LINE_FALLOFF 1
#define BATTERY_DEFAULT_BACKGROUND_COLOR glm::vec4(60, 60, 60, 255)

// Some logging
#define BATTERY_LOG_LEVEL_CRITICAL	spdlog::level::critical
#define BATTERY_LOG_LEVEL_ERROR		spdlog::level::err
#define BATTERY_LOG_LEVEL_WARN		spdlog::level::warn
#define BATTERY_LOG_LEVEL_INFO		spdlog::level::info
#define BATTERY_LOG_LEVEL_TRACE		spdlog::level::trace

#define BATTERY_LOG_LEVEL BATTERY_LOG_LEVEL_INFO

// File I/O
#define BATTERY_FILE_BLOCK_SIZE 1024