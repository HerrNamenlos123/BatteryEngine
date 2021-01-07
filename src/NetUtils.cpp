
#include "Battery/NetUtils.h"

#include <string>
#include <sstream>
#include <iostream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>

namespace Battery {
	namespace NetUtils {

		HTTPRequest GetHTTPRequest(const std::string& url, const std::string& certificates, const std::string& userAgent, long timeout) {

			using namespace curlpp::options;

			try {
				curlpp::Cleanup cleanup;
				curlpp::Easy request;

				// Set the URL.
				request.setOpt<Url>(url);
				request.setOpt<CaInfo>(certificates);
				request.setOpt<UserAgent>(userAgent);
				request.setOpt<Timeout>(timeout);

				std::ostringstream response;
				request.setOpt(new curlpp::options::WriteStream(&response));

				request.perform();

				long status = curlpp::infos::ResponseCode::get(request);

				HTTPRequest r;
				r.url = url;
				r.responseCode = status;
				r.response = response.str();

				if (status != 200) {
					std::cerr << "Server responded with code " << status << std::endl;

					r.valid = false;
					return r;
				}

				r.valid = true;
				return r;
			}

			catch (curlpp::RuntimeError& e)
			{
				std::cerr << "[NetUtils]: " << e.what() << std::endl;

				HTTPRequest r;
				r.url = url;
				r.responseCode = 0;
				r.response = "";
				r.valid = false;
				return r;
			}

			catch (curlpp::LogicError& e)
			{
				std::cerr << e.what() << std::endl;

				HTTPRequest r;
				r.url = url;
				r.responseCode = 0;
				r.response = "";
				r.valid = false;
				return r;
			}
		}

	}
}

