
#include "Battery/pch.h"
#include "Battery/Core/Exception.h"

namespace Battery {

	Exception::Exception(const std::string& msg) : std::exception(msg.c_str()) {

	}

}
