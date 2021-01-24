#pragma once

#include <string>

namespace Battery {
	namespace NetUtils {

		struct HTTPRequest {
			std::string url;
			std::string response;
			bool valid = false;
			int responseCode = 0;
		};

		/// <summary>
		/// Send a GET Request to the specified url and return the result by blocking
		/// </summary>
		/// <param name="url">- The url to send the request to, e.g. "https://example.com/some/path"</param>
		/// <param name="certificates">- A file containing one or more public certificates of 
		/// the server between "----BEGIN CERTIFICATE----" and "----END CERTIFICATE----"</param>
		/// <param name="userAgent">- Identifier for the aim of your request, for example "gister" when accessing the Github API</param>
		/// <param name="timeout">- The timeout in seconds, defaults to 10</param>
		/// <returns>NetUtils::HTTPRequest struct, which contains all informations about the response</returns>
		HTTPRequest GetHTTPRequest(const std::string& url, const std::string& certificates, const std::string& userAgent, long timeout = 10);

		//size_t GetSizeOfDownloadFile(const std::string& url, const std::string& certificates, const std::string& userAgent, long timeout = 10);

		/*class FileDownloader {
		public:
			bool download(const std::string& url, const std::string& outputFile, const std::string& caFile);
		};

		bool DownloadFile(const std::string& url, const std::string& outputFile, const std::string& caFile);*/

	}
}
