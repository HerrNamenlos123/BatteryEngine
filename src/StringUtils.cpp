
#include "Battery/StringUtils.h"

namespace Battery {
	namespace StringUtils {

		std::vector<std::string> SplitString(std::string str, char delimeter) {
			std::string::size_type b = 0;
			std::vector<std::string> result;

			while ((b = str.find_first_not_of(delimeter, b)) != std::string::npos) {
				auto e = str.find_first_of(delimeter, b);
				result.push_back(str.substr(b, e - b));
				b = e;
			}

			return result;
		}

		std::string JoinStrings(std::vector<std::string> strings, std::string spacer) {
			std::string str = "";

			for (size_t i = 0; i < strings.size(); i++) {
				str += strings[i];

				if (i < strings.size() - 1 && spacer != "") {
					str += spacer;
				}
			}

			return str;
		}

	}
}
