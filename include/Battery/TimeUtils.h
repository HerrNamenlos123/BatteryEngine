#pragma once

#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

namespace Battery {
	namespace TimeUtils {

		/// <summary>
		/// Convert a Unix timestamp like '2021-01-02T10:34:52Z' to an epoch number, which is 1609580092 in this case.
		/// Returns 0 on failure
		/// </summary>
		/// <param name="timestamp">- Unix timestamp in string format</param>
		/// <returns>std::time_t - The epoch time in seconds or 0 on failure</returns>
		std::time_t ConvertTimetamp(const std::string& timestamp);

		/// <summary>
		/// Get the current time in microseconds as Unix timestamp or epoch time
		/// </summary>
		/// <returns>uint32_t - epoch time in microseconds</returns>
		uint32_t GetMicroseconds();

	}
}
