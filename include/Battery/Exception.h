#pragma once

#include <exception>
#include <string>

namespace Battery {

	/// <summary>
	/// Custom exception class used in the entire engine
	/// </summary>
	class Exception : public std::exception {
	public:
		Exception(const std::string& msg);
	};

}
