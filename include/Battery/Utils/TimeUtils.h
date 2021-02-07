#pragma once

#include "Battery/pch.h"

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
		/// Get the time in seconds since Allegro has been initalized, 
		/// Precision goes down to the microseconds range. If not initialized, returns 0.
		/// </summary>
		/// <returns>double - Time since engine is running in seconds</returns>
		double GetRuntime();

		/// <summary>
		/// Put the current thread to sleep for x microseconds.
		/// </summary>
		/// <param name="microseconds">- Time to sleep in microseconds</param>
		void SleepMicroseconds(std::time_t microseconds);

	}
}
