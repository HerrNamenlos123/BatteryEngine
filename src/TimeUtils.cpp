
#include "Battery/TimeUtils.h"
#include <chrono>
#include <thread>

namespace Battery {
	namespace TimeUtils {

		std::time_t ConvertTimetamp(const std::string& timestamp) {
			std::istringstream ss(timestamp);

			std::tm t{};
			ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");

			if (ss.fail()) {
				return 0;
			}

			return mktime(&t);
		}

		std::time_t GetMicroseconds() {
			using namespace std::chrono;
			return (std::time_t)duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
		}

		void SleepMicroseconds(std::time_t microseconds) {
			std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
		}

	}
}
