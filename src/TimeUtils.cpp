
#include "Battery/TimeUtils.h"

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

	}
}
